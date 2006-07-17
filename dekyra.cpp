/* DeKyra - Basic Kyrandia script disassembler
 * Copyright (C) 2004  Johannes Schickel
 * Copyright (C) 2004-2006  The ScummVM Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "dekyra.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Use:\n"
			   "%s filename\n"
			   "-t   displays text segment\n"
			   "-n   display only function 'n'. n should be a integer\n"
			   "-e   set engine version (1 for kyra1 (default), 2 for kyra2)\n",
			   argv[0]);
			   
		return false;
	}
	
	uint32 file = 0xFFFFFFFE;
	bool displayText = false;
	bool displayOnlyOneScript = false;
	int32 scriptnum = 0, engine = 1;
	
	// search for some parameters
	for (int param = 1; param < argc; ++param) {
		if (*argv[param] != '-' && file == 0xFFFFFFFE)
			file = param;
		else {
			if(argv[param][1] == 't')
				displayText = true;
			else if (argv[param][1] == 'n') {
				scriptnum = atoi(argv[param+1]);
				if (scriptnum >= 0 && scriptnum < 256)
					displayOnlyOneScript = true;
				param++;
			} else if (argv[param][1] == 'e') {
				engine = atoi(argv[param+1]);
				param++;
				
			}
		}
	}
	
	if (file == 0xFFFFFFFE) {
		printf("Use:\n"
			   "%s filename\n"
			   "-t   displays text segment\n"
			   "-n   display only function 'n'\n"
			   "-e   set engine version (1 for kyra1 (default), 2 for kyra2)\n",
			   argv[0]);
			   
		return false;
	}
	
	Script myscript(argv[file]);

	
	if (myscript.isOpen()) {
		if (displayText) {
			myscript.decodeTextArea();
			printf("\n\n");
			return true;
		}
		
		if (!displayOnlyOneScript) {
			myscript.decodeScript(engine);
		} else {
			if (!myscript.decodeSpecialScript(scriptnum, engine)) {
				error("No script with number %d", scriptnum);
			}
		}
		
	} else {
		error("Couldn't find file '%s'", argv[1]);
	}	
	
	return true;
}

Script::Script(const char* filename) {
	_scriptFile = 0;
	_stackPos = 0;
	FILE* script = fopen(filename, "rb");
	
	if (!script)
		return;
		
	_scriptSize = fileSize(script);
  	_scriptFile = new uint8[_scriptSize];
	assert(_scriptFile); 
	fread(_scriptFile, sizeof(uint8) * _scriptSize, 1, script);
	fseek(script, 0, SEEK_SET);
 	
	uint8 chunkName[sizeof("EMC2ORDR") + 1];
	
	// so lets look for our chunks :)
	while(true) {
		if ((uint32)ftell(script) >= _scriptSize) {
			break;
		}		
		// lets read only the first 4 chars
		fread(chunkName, sizeof(uint8) * 4, 1, script);
		chunkName[4] = '\0';
		// check name of chunk
		if (!strcmp((char*)chunkName, "FORM")) {			
			// FreeKyra swaps the size I only read it in BigEndian :)
			_chunks[kForm]._size = readUint32BE(script);				
		} else if (!strcmp((char*)chunkName, "TEXT")) {
			uint32 text_size = readUint32BE(script);
			text_size += text_size % 2 != 0 ? 1 : 0;
			
			_chunks[kText]._data = _scriptFile + ftell(script);
			_chunks[kText]._size = READ_BE_UINT16(_chunks[kText]._data) >> 1;
			_chunks[kText]._additional = _chunks[kText]._data + (_chunks[kText]._size << 1);				
			fseek(script, text_size, SEEK_CUR);
		} else if (!strcmp((char*)chunkName, "DATA")) {
			_chunks[kData]._size = readUint32BE(script);
			_chunks[kData]._data = _scriptFile + ftell(script);
			// mostly it will be the end of the file because all files should end with a 'DATA' chunk
			fseek(script, _chunks[kData]._size, SEEK_CUR);
		} else {
			// read next 4 chars
			fread(&chunkName[4], sizeof(uint8) * 4, 1, script);
			chunkName[8] = '\0';
			if (!strcmp((char*)chunkName, "EMC2ORDR")) {
				_chunks[kEmc2Ordr]._size = readUint32BE(script) >> 1;
				_chunks[kEmc2Ordr]._data = _scriptFile + ftell(script);					
				fseek(script, _chunks[kEmc2Ordr]._size * 2, SEEK_CUR);
			} else {
				// any unkown chunk or problems with seeking through the file
				error("unknown chunk '%s'", chunkName);
			}
		}
	}
	
	fclose(script);
}

void Script::decodeTextArea(void) {
	printf("TEXT chunk:\n");
	// first is size
	for (uint32 pos = 1; pos < (_chunks[kText]._size << 1); ++pos) {
		if (READ_BE_UINT16(_chunks[kText]._data + 2*pos) >= _scriptSize) {
			break;
		}
		uint32 startoffset = READ_BE_UINT16(_chunks[kText]._data + 2*pos);
		printf("%d(at %d) : %s\n", pos, startoffset, (char*)(_chunks[kText]._data + startoffset));
	}
}

void Script::decodeScript(int8 engine) {
	decodeSpecialScript(-1, engine);
}

struct CommandDesc {
	uint16 command;	// normally uint8
	const char* description;
	bool usesArgument;	
};

struct ScriptDesc {
	int32 script;	// normally uint8
	const char* description;
};

struct OpcodeDesc {
	uint16 opcode;	// normally uint8
	const char* description;
};

// function which calls opcode procs
const uint16 OPCODE_CALLER = 0x0E;

bool Script::decodeSpecialScript(int32 script, int8 engineVersion) {
	static CommandDesc commandDesc[] = {
		{ 0x00, "goToLine", true },
		{ 0x01, "setReturn", true },
		{ 0x02, "pushRetRec", true },
		{ 0x03, "push", true },
		{ 0x04, "push", true },
		{ 0x05, "pushVar", true },
		{ 0x06, "pushFrameNeg", true },
		{ 0x07, "pushFramePos", true },
		{ 0x08, "popRetRec", true },
		{ 0x09, "popVar", true },
		{ 0x0A, "popFrameNeg", true },
		{ 0x0B, "popFramePos", true },
		{ 0x0C, "addToSP", true },
		{ 0x0D, "subFromSP", true },
		{ 0x0E, "execOpcode", true },
		{ 0x0F, "ifNotGoTo", true },
		{ 0x10, "negate", true },
		{ 0x11, "evaluate", true },
		// normally only until 0xFF
		{ 0xFFFF, 0, 0 }
	};
	
	static ScriptDesc scriptDesc[] = {
		{ 0, "kSetupScene" },
		{ 1, "kClickEvent" },
		{ 2, "kInitScreenEvent" },
		{ 3, "kInitDataEvent" },
		{ 4, "kEnterEvent" },
		{ 5, "kExitEvent" },
		{ 6, "kClick2Event" },
		{ 7, "kLoadResources" },
		{ 0xFFFF, "unknown script" }
	};
	
	static OpcodeDesc kyra2OpcodeDesc[] = {
		{ 0x0, "o2_setCharacterShape" },
		{ 0x1, "o2_drawShapeFrame" },
		{ 0x2, "o2_defineObject" },
		{ 0x3, "o2_setCharacterFrame" },
		{ 0x4, "o2_getCharacterPosX" },
		{ 0x5, "o2_getCharacterPosY" },
		{ 0x6, "o2_getCharacterField5" },
		{ 0x7, "o2_getCharacterSceneId" },
		{ 0x8, "o2_setSceneCommentString" },
		{ 0x9, "o2_isBitSet" },
		{ 0xa, "o2_setBit" },
		{ 0xb, "o2_setCharacterField6" },
		{ 0xc, "o2_setCharacterField5" },
		{ 0xd, "o2_restartToChapter1?" },
		{ 0xe, "o2_changeCharacterDirection" },
		{ 0xf, "o2_unk0x0F" },
		{ 0x10, "o2_unk0x10" },
		{ 0x11, "o2_setupNewChapter" },
		{ 0x12, "o2_copyScreenPage" },
		{ 0x13, "o2_printText" },
		{ 0x14, "o2_closeWSAFile" },
		{ 0x15, "o2_copyPage" },
		{ 0x16, "o2_copyScreenPageAlt" },
		{ 0x17, "o2_displayWSASequentialFrames" },
		{ 0x18, "o2_runWSAFrames" },
		{ 0x19, "o2_openWSAFile" },
		{ 0x1a, "o2_displayWSAFrame" },
		{ 0x1b, "o2_runWSAFromBeginningToEnd" },
		{ 0x1c, "o2_addItemToInventory" },
		{ 0x1d, "o2_animateShape" },
		{ 0x1e, "o2_unk0x1E" },
		{ 0x1f, "o2_setMouseBounds" },
		{ 0x20, "o2_unk0x20" },
		{ 0x21, "o2_dummy" },
		{ 0x22, "o2_removeItemFromInventory" },
		{ 0x23, "o2_defineBox?" },
		{ 0x24, "o2_reorderInventory" },
		{ 0x25, "o2_specificItemInInventory" },
		{ 0x26, "o2_getBox" },
		{ 0x27, "o2_queryGameState" },
		{ 0x28, "o2_clearGameState" },
		{ 0x29, "o2_setGameState" },
		{ 0x2a, "o2_createMouseItem" },
		{ 0x2b, "o2_destroyMouseItem" },
		{ 0x2c, "o2_mouseIsPointer" },
		{ 0x2d, "o2_hideMouse" },
		{ 0x2e, "o2_defineRoomDoor" },
		{ 0x2f, "o2_setMousePos" },
		{ 0x30, "o2_showMouse" },
		{ 0x31, "o2_scrollScreen?" },
		{ 0x32, "o2_wipeDownMouseItem" },
		{ 0x33, "o2_getElapsedSeconds" },
		{ 0x34, "o2_getTimerData" },
		{ 0x35, "o2_delayIf6or8" },
		{ 0x36, "o2_delay" },
		{ 0x37, "o2_delayWithOptionalInput" },
		{ 0x38, "o2_setTimerDataTicks" },
		{ 0x39, "o2_setTimerData" },
		{ 0x3a, "o2_setScaleSlot" },
		{ 0x3b, "o2_setDepthSlot" },
		{ 0x3c, "o2_setRoomHotSpotX" },
		{ 0x3d, "o2_loadShape" },
		{ 0x3e, "o2_drawShapeToScreenAndAnimate" },
		{ 0x3f, "o2_drawShapeToScreen" },
		{ 0x40, "o2_unsetSpriteFlag" },
		{ 0x41, "o2_setSpriteFlag" },
		{ 0x42, "o2_dummy" },
		{ 0x43, "o2_loadPalette" },
		{ 0x44, "o2_fadePalette" },
		{ 0x45, "o2_copyDisplayedPage" },
		{ 0x46, "o2_cmd_copyWSARegion" },
		{ 0x47, "o2_unsetSceneFlagBit1" },
		{ 0x48, "o2_setSceneFlagBit1" },
		{ 0x49, "o2_copyScreenPage2" },
		{ 0x4a, "o2_setSceneOffsets" },
		{ 0x4b, "o2_delayCount" },
		{ 0x4c, "o2_changePalette" },
		{ 0x4d, "o2_setCustomPaletteRange" },
		{ 0x4e, "o2_dummy" },
		{ 0x4f, "o2_dummy" },
		{ 0x50, "o2_unk0x50" },
		{ 0x51, "o2_enterNewScene" },
		{ 0x52, "o2_isBlockedOutRegion" },
		{ 0x53, "o2_setScaleMode" },
		{ 0x54, "o2_getRoomExit" },
		{ 0x55, "o2_setRoomHotSpotY" },
		{ 0x56, "o2_setCharacterField6FromPosition" },
		{ 0x57, "o2_dummy" },
		{ 0x58, "o2_unk0x58" },
		{ 0x59, "o2_unk0x59" },
		{ 0x5a, "o2_setSceneUnkData" },
		{ 0x5b, "o2_blockInWalkableRegion" },
		{ 0x5c, "o2_blockOutWalkableRegion" },
		{ 0x5d, "o2_unk0x5D" },
		{ 0x5e, "o2_unk0x5E" },
		{ 0x5f, "o2_unk0x5F" },
		{ 0x60, "o2_random" },
		{ 0x61, "o2_unk0x61" },
		{ 0x62, "o2_freezeMouse" },
		{ 0x63, "o2_setShapeScaledFlag" },
		{ 0x64, "o2_setQuitFlag" },
		{ 0x65, "o2_unk0x65" },
		{ 0x66, "o2_shakeScreen" },
		{ 0x67, "o2_fillRect" },
		{ 0x68, "o2_getKey" },
		{ 0x69, "o2_printTextInBox" },
		{ 0x6a, "o2_dummy" },
		{ 0x6b, "o2_waitForConfirmationMouseClick" },
		{ 0x6c, "o2_swapScreenPages" },
		{ 0x6d, "o2_setSceneDimensions" },
		{ 0x6e, "o2_unk0x6E" },
		{ 0x6f, "o2_setTimer" },
		{ 0x70, "o2_loadSceneDataAlt" },
		{ 0x71, "o2_unk0x71" },
		{ 0x72, "o2_defineSprite" },
		{ 0x73, "o2_scrollScene?" },
		{ 0x74, "o2_runNPCSubscript" },
		{ 0x75, "o2_unk0x75" },
		{ 0x76, "o2_unk0x76" },
		{ 0x77, "o2_setCharacterField2" },
		{ 0x78, "o2_getCharacterField2" },
		{ 0x79, "o2_defineRoom" },
		{ 0x7a, "o2_unk0x7A" },
		{ 0x7b, "o2_setUnkGameFlag0" },
		{ 0x7c, "o2_getUnkGameFlag0" },
		{ 0x7d, "o2_dummy" },
		{ 0x7e, "o2_dummy" },
		{ 0x7f, "o2_unk0x7F" },
		{ 0x80, "o2_unk0x80" },
		{ 0x81, "o2_startNewChapter" },
		{ 0x82, "o2_getUnkGameFlag1" },
		{ 0x83, "o2_setUnkGameFlag1" },
		{ 0x84, "o2_getUnkGameFlag2" },
		{ 0x85, "o2_setUnkGameFlag2" },
		{ 0x86, "o2_getUnkGameFlag3" },
		{ 0x87, "o2_setUnkGameFlag3" },
		{ 0x88, "o2_unk0x88" },
		{ 0x89, "o2_unk0x89" },
		{ 0x8a, "o2_loadSpriteTimerData" },
		{ 0x8b, "o2_unk0x8B" },
		{ 0x8c, "o2_unk0x8C" },
		{ 0x8d, "o2_unk0x8D" },
		{ 0x8e, "o2_unk0x8E" },
		{ 0x8f, "o2_setTimerFlag" },
		{ 0x90, "o2_unsetTimerFlag" },
		{ 0x91, "o2_getTimerFlag" },
		{ 0x92, "o2_unk0x92" },
		{ 0x93, "o2_unk0x93" },
		{ 0x94, "o2_unk0x94" },
		{ 0x95, "o2_processPalette" },
		{ 0x96, "o2_closeWSAFile" },
		{ 0x97, "o2_playMeanwhileScene" },
		{ 0x98, "o2_unk0x98" },
		{ 0x99, "o2_unk0x99" },
		{ 0x9a, "o2_loadSceneData" },
		{ 0x9b, "o2_unk0x9B" },
		{ 0x9c, "o2_unk0x9C" },
		{ 0x9d, "o2_unk0x9D" },
		{ 0x9e, "o2_setTimerDataAndUpdateTimings" },
		{ 0x9f, "o2_setPaletteColor" },
		{ 0xa0, "o2_unk0xA0" },
		{ 0xa1, "o2_getStateData" },
		{ 0xa2, "o2_unk0xA2" },
		{ 0xa3, "o2_ifNotZero" },
		{ 0xa4, "o2_getDelayMode" },
		{ 0xa5, "o2_true" },
		{ 0xa6, "o2_unk0xA6" },
		{ 0xa7, "o2_dummy" }
	};
 	 
	static OpcodeDesc kyra1OpcodeDesc[] = {
		{ 0x00 ,"o1_Magic_In_Mouse_Item" },
		{ 0x01 ,"o1_Character_Says" },
		{ 0x02 ,"o1_Pause_Ticks" },
		{ 0x03 ,"o1_Draw_Scene_Anim_Shape" },
		{ 0x04 ,"o1_Query_Game_Flag" },
		{ 0x05 ,"o1_Set_Game_Flag" },
		{ 0x06 ,"o1_Reset_Game_Flag" },
		{ 0x07 ,"o1_Run_NPC_Script" },
		{ 0x08 ,"o1_Set_Special_Exit_List" },
		{ 0x09 ,"o1_Block_In_Walkable_Region" },
		{ 0x0A ,"o1_Block_Out_Walkable_Region" },
		{ 0x0B ,"o1_Walk_Player_To_Point" },
		{ 0x0C ,"o1_Drop_Item_In_Scene" },
		{ 0x0D ,"o1_Draw_Anim_Shape_Into_Scene" },
		{ 0x0E ,"o1_Create_Mouse_Item" },
		{ 0x0F ,"o1_Save_Page_To_Disk" },
		{ 0x10 ,"o1_Scene_Anim_On" },
		{ 0x11 ,"o1_Scene_Anim_Off" },
		{ 0x12 ,"o1_Elapsed_Seconds" },
		{ 0x13 ,"o1_Mouse_Is_Pointer" },
		{ 0x14 ,"o1_Destroy_Mouse_Item" },
		{ 0x15 ,"o1_Run_Scene_Anim_Until_Done" },
		{ 0x16 ,"o1_Fade_Special_Palette" },
		{ 0x17 ,"o1_Play_AdLib_Sound" },
		{ 0x18 ,"o1_Play_AdLib_Score" },
		{ 0x19 ,"o1_Phase_In_Same_Scene" },
		{ 0x1a ,"o1_Set_Scene_Phasing_Flag" },
		{ 0x1b ,"o1_Reset_Scene_Phasing_Flag" },
		{ 0x1c ,"o1_Query_Scene_Phasing_Flag" },
		{ 0x1d ,"o1_Scene_To_Direction" },
		{ 0x1e ,"o1_Set_Birthstone_Gem" },
		{ 0x1f ,"o1_Place_Item_In_Generic_Map_Scene" },
		{ 0x20 ,"o1_Set_Brandon_Status_Bit" },
		{ 0x21 ,"o1_Pause_Seconds" },
		{ 0x22 ,"o1_Get_Characters_Location" },
		{ 0x23 ,"o1_Run_NPC_Subscript" },
		{ 0x24 ,"o1_Magic_Out_Mouse_Item" },
		{ 0x25 ,"o1_Internal_Anim_On" },
		{ 0x26 ,"o1_Force_Brandon_To_Normal" },
		{ 0x27 ,"o1_Poison_Death_Now" },
		{ 0x28 ,"o1_Set_Scale_Mode" },
		{ 0x29 ,"o1_Open_WSA_File" },
		{ 0x2a ,"o1_Close_WSA_File" },
		{ 0x2b ,"o1_Run_WSA_From_Beginning_To_End" },
		{ 0x2c ,"o1_Display_WSA_Frame" },
		{ 0x2d ,"o1_Enter_New_Scene" },
		{ 0x2e ,"o1_Set_Special_Enter_X_And_Y" },
		{ 0x2f ,"o1_Run_WSA_Frames" },
		{ 0x30 ,"o1_Pop_Brandon_Into_Scene" },
		{ 0x31 ,"o1_Restore_All_Object_Backgrounds" },
		{ 0x32 ,"o1_Set_Custom_Palette_Range" },
		{ 0x33 ,"o1_Load_Page_From_Disk" },		
		{ 0x34 ,"o1_Custom_Print_Talk_String" },
		{ 0x35 ,"o1_Restore_Custom_Print_Background" },
		{ 0x36 ,"o1_Hide_Mouse" },
		{ 0x37 ,"o1_Show_Mouse" },
		{ 0x38 ,"o1_Get_Character_X" },
		{ 0x39 ,"o1_Get_Character_Y" },
		{ 0x3A ,"o1_Change_Characters_Facing" },
		{ 0x3B ,"o1_Copy_WSA_Region" },
		{ 0x3C ,"o1_Text_Print" },
		{ 0x3D ,"o1_Random" },
		{ 0x3E ,"o1_Load_Sound_File" },
		{ 0x3F ,"o1_Display_WSA_Frame_On_HidPage" },
		{ 0x40 ,"o1_Display_WSA_Sequential_Frames" },
		{ 0x41 ,"o1_Draw_Character_Standing" },
		{ 0x42 ,"o1_Internal_Anim_Off" },
		{ 0x43 ,"o1_Change_Characters_X_And_Y" },
		{ 0x44 ,"o1_Clear_Scene_Animator_Beacon" },
		{ 0x45 ,"o1_Query_Scene_Animator_Beacon" },
		{ 0x46 ,"o1_Refresh_Scene_Animator" },
		{ 0x47 ,"o1_Place_Item_In_Off_Scene" },
		{ 0x48 ,"o1_Wipe_Down_Mouse_Item" },
		{ 0x49 ,"o1_Place_Character_In_Other_Scene" },
		{ 0x4A ,"o1_Get_Key" },
		{ 0x4B ,"o1_Specific_Item_In_Inventory" },
		{ 0x4C ,"o1_Pop_Mobile_NPC_Into_Scene" },
		{ 0x4D ,"o1_Mobile_Character_In_Scene" },
		{ 0x4E ,"o1_Hide_Mobile_Character" },
		{ 0x4F ,"o1_Unhide_Mobile_Character" },
		{ 0x50 ,"o1_Set_Characters_Location" },
		{ 0x51 ,"o1_Walk_Character_To_Point" },
		{ 0x52 ,"o1_Special_Event_Display_Brynns_Note" },
		{ 0x53 ,"o1_Special_Event_Remove_Brynns_Note" },
		{ 0x54 ,"o1_Set_Logic_Page" },
		{ 0x55 ,"o1_Fat_Print" },
		{ 0x56 ,"o1_Preserve_All_Object_Backgrounds" },
		{ 0x57 ,"o1_Update_Scene_Animations" },
		{ 0x58 ,"o1_Scene_Animation_Active" },
		{ 0x59 ,"o1_Set_Characters_Movement_Delay" },
		{ 0x5A ,"o1_Get_Characters_Facing" },
		{ 0x5B ,"o1_Bkgd_Scroll_Scene_And_Masks_Right" },
		{ 0x5C ,"o1_Dispel_Magic_Animation" },
		{ 0x5d ,"o1_Find_Brightest_Fireberry" },
		{ 0x5e ,"o1_Set_Fireberry_Glow_Palette" },
		{ 0x5f ,"o1_Set_Death_Handler_Flag" },
		{ 0x60 ,"o1_Drink_Potion_Animation" },
		{ 0x61 ,"o1_Make_Amulet_Appear" },
		{ 0x62 ,"o1_Draw_Item_Shape_Into_Scene" },
		{ 0x63 ,"o1_Set_Characters_Current_Frame" },
		{ 0x64 ,"o1_Wait_For_Confirmation_Mouse_Click" },
		{ 0x65 ,"o1_Page_Flip" },
		{ 0x66 ,"o1_Set_Scene_File" },
		{ 0x67 ,"o1_What_Item_In_Marble_Vase" },
		{ 0x68 ,"o1_Set_Item_In_Marble_Vase" },
		{ 0x69 ,"o1_Add_Item_To_Inventory" },
		{ 0x6a ,"o1_Int_Print" },
		{ 0x6b ,"o1_Shake_Screen" },
		{ 0x6c ,"o1_Create_Amulet_Jewel" },
		{ 0x6d ,"o1_Set_Scene_Anim_Curr_XY" },
		{ 0x6e ,"o1_Poison_Brandon_And_Remaps" },
		{ 0x6f ,"o1_Fill_Flask_With_Water" },
		{ 0x70 ,"o1_Get_Characters_Movement_Delay" },
		{ 0x71 ,"o1_Get_Birthstone_Gem" },
		{ 0x72 ,"o1_Query_Brandon_Status_Bit" },
		{ 0x73 ,"o1_Play_Flute_Animation" },
		{ 0x74 ,"o1_Play_Winter_Scroll_Sequence" },
		{ 0x75 ,"o1_Get_Idol_Gem" },
		{ 0x76 ,"o1_Set_Idol_Gem" },
		{ 0x77 ,"o1_Total_Items_In_Scene" },
		{ 0x78 ,"o1_Restore_Brandons_Movement_Delay" },
		{ 0x79 ,"o1_Set_Mouse_Pos" },
		{ 0x7a ,"o1_Get_Mouse_State" },
		{ 0x7b ,"o1_Set_Entrance_Mouse_Cursor_Track" },
		{ 0x7c ,"o1_Item_Appears_On_Ground" },
		{ 0x7d ,"o1_Set_No_Draw_Shapes_Flag" },
		{ 0x7e ,"o1_Fade_Entire_Palette" },
		{ 0x7f ,"o1_Item_On_Ground_Here" },
		{ 0x80 ,"o1_Query_Cauldron_State" },
		{ 0x81 ,"o1_Set_Cauldron_State" },
		{ 0x82 ,"o1_Query_Crystal_State" },
		{ 0x83 ,"o1_Set_Crystal_State" },
		{ 0x84 ,"o1_Set_Palette_Range" },
		{ 0x85 ,"o1_Shrink_Brandon_Down" },
		{ 0x86 ,"o1_Grow_Brandon_Up" },
		{ 0x87 ,"o1_Set_Brandon_Scale_X_And_Y" },
		{ 0x88 ,"o1_Reset_Scale_Mode" },
		{ 0x89 ,"o1_Get_Scale_Depth_Table_Value" },
		{ 0x8a ,"o1_Set_Scale_Depth_Table_Value" },
		{ 0x8b ,"o1_Message" },
		{ 0x8c ,"o1_Check_Click_On_NPC" },
		{ 0x8d ,"o1_Get_Foyer_Item" },
		{ 0x8e ,"o1_Set_Foyer_Item" },
		{ 0x8F ,"o1_Set_No_Item_Drop_Region" },
		{ 0x90 ,"o1_Walk_Malcolm_On" },
		{ 0x91 ,"o1_Passive_Protection" },
		{ 0x92 ,"o1_Set_Playing_Loop" },
		{ 0x93 ,"o1_Brandon_To_Stone_Sequence" },
		{ 0x94 ,"o1_Brandon_Healing_Sequence" },
		{ 0x95 ,"o1_Protect_Command_Line" },
		{ 0x96 ,"o1_Pause_Music_Seconds" },
		{ 0x97 ,"o1_Reset_Mask_Region" },
		{ 0x98 ,"o1_Set_Palette_Change_Flag" },
		{ 0x99 ,"o1_Fill_Rect" },
		{ 0x9a ,"o1_Voc_Unload" },
		{ 0x9b ,"o1_Voc_Load" },
		{ 0x9c ,"o1_Dummy" }
	};
	
	if (script > -1) {
		_instructionPos = (READ_BE_UINT16(_chunks[kEmc2Ordr]._data + 2 * script) << 1);	
	} else
		_instructionPos = 0;	
	
	memset(_stack, 0, sizeof(_stack));
	memset(_registers, 0, sizeof(_registers));
	
	_stackPos = 0;

	uint8* script_start = _chunks[kData]._data;
	bool gotArgument = true;
	
	// uint32 nextScriptStartsAt = getNextScriptPos(_instructionPos);
	
	while(true) {
		if ((uint32)_instructionPos > _chunks[kData]._size /*|| (uint32)_instructionPos >= nextScriptStartsAt*/) {
			break;
		}
		
		for (uint32 pos = 0; pos < ARRAYSIZE(scriptDesc) - 1; ++pos) {
			if ((READ_BE_UINT16(_chunks[kEmc2Ordr]._data + 2*pos) << 1) == _instructionPos) {
				printf("\nScript %i: %s:\n", pos, scriptDesc[pos].description);
				break;
			}
		}

		// prints the offset
		printf("0x%04x:\t\t" , _instructionPos);
			
		gotArgument = true;
		_currentCommand = *(script_start + _instructionPos++);
			
		// gets out 
		if (_currentCommand & 0x80) {
			_argument = ((_currentCommand & 0x0F) << 8) | *(script_start + _instructionPos++);
			_currentCommand &= 0xF0;
		} else if (_currentCommand & 0x40) {
			_argument = *(script_start + _instructionPos++);
		} else if (_currentCommand & 0x20) {
			_instructionPos++;
			
			// FIXME: I am not 100% sure if this code is correct (after my
			// 'endian macro' changes), somebody please double check.
			uint16 tmp = READ_BE_UINT16(script_start + _instructionPos);
			tmp &= 0xFF7F;
			
			_argument = tmp;
			_instructionPos += 2;
		} else {
			gotArgument = false;
		}
			
		_currentCommand &= 0x1f;

		
		bool gotCommand = false;
		
		// lets get out what the command means
		for (uint32 pos = 0; pos < ARRAYSIZE(commandDesc) - 1; ++pos) {
			if (commandDesc[pos].command == _currentCommand) {
				gotCommand = true;
				if (commandDesc[pos].command != OPCODE_CALLER)
					printf("\t%s" , commandDesc[pos].description);
					
				if (commandDesc[pos].usesArgument && commandDesc[pos].command != OPCODE_CALLER) {
					if (commandDesc[pos].command == 0x3 || commandDesc[pos].command == 0x4)
						printf("(0x%x | %s)" , _argument, stringAtIndex(_argument));
					else
						printf("(0x%x)\t" , _argument);
					
				} else if(commandDesc[pos].usesArgument && commandDesc[pos].command == OPCODE_CALLER) {
					bool gotOpcode = false;
					// lets look for our opcodes
					if (engineVersion == 1) 
						for (uint32 pos2 = 0; pos2 < ARRAYSIZE(kyra1OpcodeDesc); ++pos2) {
							if (kyra1OpcodeDesc[pos2].opcode == _argument) {
								printf("%s()", kyra1OpcodeDesc[pos2].description);
								gotOpcode = true;
								break;
							}
						}
					else if (engineVersion == 2)
						for (uint32 pos2 = 0; pos2 < ARRAYSIZE(kyra2OpcodeDesc); ++pos2) {
							if (kyra2OpcodeDesc[pos2].opcode == _argument) {
								printf("%s()", kyra2OpcodeDesc[pos2].description);
								gotOpcode = true;
								break;
							}
						}
					if (!gotOpcode)
						printf("UNKNOWN OPCODE: 0x%x", _argument);
				}
				
				break;
			}
		}
		execCommand(_currentCommand);
		
		// prints our command number + arg
		if (!gotCommand) {
			printf("0x%x(0x%x)", _currentCommand, _argument);
		}
		
		if (!gotArgument) {
			printf("\t; couldn't get argument! maybe command is wrong.");
		}
		
		printf("\n");
	}
	
	printf("\n-------------\n");
	
	return true;
}

uint32 Script::getNextScriptPos(uint32 current_start) {
	uint32 pos = 0xFFFFFFFE;
	
	for (uint32 tmp = 0; tmp < _chunks[kEmc2Ordr]._size; ++tmp) {
		uint32 tmp2 = READ_BE_UINT16(_chunks[kEmc2Ordr]._data + tmp);
		tmp2 = (tmp2 << 1) + 2;
		if (tmp2 > current_start && tmp2 < pos) {
			pos = tmp2;
		}
	}
	
	if (pos > _scriptSize) {
		pos = _scriptSize;
	}
	
	return pos;
}
	
const char* Script::stringAtIndex(int32 index) {
	if (index < 0 || (uint32)index >= _chunks[kText]._size)
		return 0;
	
	uint16 offset = READ_BE_UINT16(_chunks[kText]._data + 2*index);
	return (const char *)(_chunks[kText]._data + offset);
}

void Script::execCommand(uint32 command) {
#define COMMAND(x) { &Script::x }
	typedef void (Script::*CommandProc)();
	struct CommandEntry {
		CommandProc proc;
	};

	// now we create a list of all Command/Opcode procs and so
	static CommandEntry commandProcs[] = {
		// 0x00
		COMMAND(goToLine),
		COMMAND(setReturn),
		COMMAND(pushRetRec),
		COMMAND(push),
		// 0x04
		COMMAND(push),			
		COMMAND(pushVar),
		COMMAND(pushFrameNeg),
		COMMAND(pushFramePos),
		// 0x08
		COMMAND(popRetRec),
		COMMAND(popVar),
		COMMAND(popFrameNeg),
		COMMAND(popFramePos),
		// 0x0C
		COMMAND(addToSP),
		COMMAND(subFromSP),
		COMMAND(execOpcode),
		COMMAND(ifNotGoTo),
		// 0x10
		COMMAND(negate),
		COMMAND(evaluate)
	};

	static uint16 _numCommands = ARRAYSIZE(commandProcs);
	static CommandEntry* _commands = commandProcs;
	
	if (command < _numCommands) {
		CommandProc currentProc = _commands[command].proc;
		(this->*currentProc)();
	}
}

void Script::goToLine(void) {
}

void Script::setReturn(void) {
}

void Script::pushRetRec(void) {
}

void Script::push(void) {
}

void Script::pushVar(void) {
}

void Script::pushFrameNeg(void) {
}

void Script::pushFramePos(void) {
}

void Script::popRetRec(void) {
}

void Script::popVar(void) {
}

void Script::popFrameNeg(void) {
}

void Script::popFramePos(void) {
}

void Script::addToSP(void) {
}

void Script::subFromSP(void) {
}

void Script::execOpcode(void) {
}

void Script::ifNotGoTo(void) {
}

void Script::negate(void) {
}

void Script::evaluate(void) {		
	switch(_argument) {
	case 0:
		printf("x && y");
		break;
	
	case 1:
		printf("x || y");
		break;

	case 2:
		printf("x == y");
		break;
	
	case 3:
		printf("x != y");
		break;

	case 4:
		printf("x < y");
		break;

	case 5:
		printf("x <= y");
		break;
		
	case 6:
		printf("x > y");
		break;
	
	case 7:
		printf("x >= y");
		break;
	
	case 8:
		printf("x + y");
		break;
		
	case 9:
		printf("x - y");
		break;
	
	case 10:
		printf("x * y");
		break;
		
	case 11:
		printf("x / y");
		break;
	
	case 12:
		printf("x >> y");
		break;
	
	case 13:
		printf("x << y");
		break;
	
	case 14:
		printf("x & y");
		break;
	
	case 15:
		printf("x | y");
		break;
	
	case 16:
		printf("x %% y");
		break;
	
	case 17:
		printf("x ^ y");
		break;
	
	default:
		printf("ERROR: unknown evaluate command %d\n", _argument);
		break;
	};
	
}

