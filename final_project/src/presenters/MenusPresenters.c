#include "MenusAndErrPresenters.h"
#include "../services/MVPutils.h"

/*** menus PHEs ***/

/* generalMenuPHE is the PHE function used for all the menus of the program
 * (both simple and complex menus. it gets the menu model, view state, an array of states representing
 * the state the button number leads to, number of buttons in the menu, the stateId, the current button selected
 * the current value of the values button (if applicable), and the max value of the values button (if applicable) */
StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	/* get the logical event, viewState and menuModel */
	logicalEventRef menuEvent = logicalEvent;
	ViewStateref menuView = viewState;
	MenuDataRef menuModel = menuModel;
	/* switch over event types */
	switch(menuEvent->type){
		case(DO_QUIT): /* exit the program */
			returnStateId = QUIT;
			break;
		case(SELECT_CURR_BUTTON):
			returnStateId = states[*currButton]; /* set the correct return stateId according to the button and state arr*/
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(menuView->buttonsArr[*currButton],
					menuView->buttonsArr[(*currButton+1)%numOfButtons]); /* update the selected button in the view */
			*currButton = (*currButton + 1)%numOfButtons; /* update the button number */
			menuView->currButton = *currButton;/* update the current button in the view */
			break;
		case(SELECT_BUTTON_NUM):
			*currButton = menuEvent->buttonNum; /* get the number of button from the event */
			returnStateId = states[menuEvent->buttonNum]; /* set the correct return stateId according to the button and state arr*/
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(MARK_VALUES_BUTTON):
			changeSelectedButton(menuView->buttonsArr[*currButton],
					menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(INCREASE_VALUE):
			increaseValuesButton(currValue, maxValue, menuView->buttonsArr[FIRST_BUTTON]); /* update the value */
			changeSelectedButton(menuView->buttonsArr[*currButton],
				menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(DECREASE_VALUE):
			decreaseValuesButton(currValue, MIN_VALUE, menuView->buttonsArr[FIRST_BUTTON]); /* update the value */
			changeSelectedButton(menuView->buttonsArr[*currButton],
				menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(NO_EVENT):
			break;
		default:
			break;
	}
	free(logicalEvent); /* free logical event */
	return returnStateId;
}

/* the phe function for main menu. calls generalMenuPHE */
StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MAIN_MENU; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef mainMenuModel = model;
	/* create an array of return stateIds according to the buttons: */
	StateId mainMenuStates[MAIN_MENU_NUM_BUTTONS] = {CHOOSE_CAT, LOAD_GAME, WORLD_BUILDER, EDIT_GAME, QUIT};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mainMenuStates, MAIN_MENU_NUM_BUTTONS, returnStateId,
			&mainMenuModel->mainMenuButton, NULL, 0);
	if (returnStateId == WORLD_BUILDER) /* set edited world to 0 if we're going to world builder from main menu */
		mainMenuModel->editedWorld = 0;
	mainMenuModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for choose cat. Calls generalMenuPHE */
StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_CAT; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseCatModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId chooseCatStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, chooseCatModel->preChooseCat};
	if (chooseCatModel->preChooseCat == PLAY_GAME) /* set the first button to go to play game if we came from play game */
		chooseCatStates[0]=PLAY_GAME;
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseCatStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseCatModel->chooseCatButton, NULL, 0);
	if (returnStateId == CHOOSE_MOUSE){ /* human was pressed and we are before playing the game */
		chooseCatModel->isCatHuman = 1;
		chooseCatModel->preChooseMouse = CHOOSE_CAT;
	}
	else if (returnStateId == PLAY_GAME && chooseCatModel->chooseCatButton == 0){ /* human was pressed and we are in play game */
			chooseCatModel->isCatHuman = 1;
			chooseCatModel->catSkill = DEFAULT_SKILL;
	}
	else if (returnStateId == CAT_SKILL) /* update the current skill in a temp value */
		chooseCatModel->currValueTemp = chooseCatModel->catSkill;
	else if (returnStateId == LOAD_GAME) /* back was pressed */
		chooseCatModel->chooseCatButton = 0;
	chooseCatModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for choose mouse. Calls generalMenuPHE */
StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_MOUSE; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseMouseModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId chooseMouseStates[COMMON_MENU_NUM_BUTTONS] = {PLAY_GAME, MOUSE_SKILL, chooseMouseModel->preChooseMouse};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseMouseStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseMouseModel->chooseMouseButton, NULL, 0);
	if (returnStateId == PLAY_GAME && chooseMouseModel->chooseMouseButton == 0){ /* human was pressed */
		chooseMouseModel->isMouseHuman = 1;
		chooseMouseModel->mouseSkill = DEFAULT_SKILL;
	}
	else if (returnStateId == MOUSE_SKILL) /* update the current skill in a temp value */
		chooseMouseModel->currValueTemp = chooseMouseModel->mouseSkill;
	else if (returnStateId == CHOOSE_CAT || returnStateId == CAT_SKILL) /* back was pressed */
		chooseMouseModel->chooseMouseButton = 0;
	chooseMouseModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for cat skill. Calls generalMenuPHE */
StateId catSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CAT_SKILL; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef catSkillModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId catSkillStates[COMMON_MENU_NUM_BUTTONS] = {CAT_SKILL, CHOOSE_MOUSE, CHOOSE_CAT};
	if (catSkillModel->preChooseCat == PLAY_GAME)
		catSkillStates[1]=PLAY_GAME; /* set the second button to go to play game if we came from play game */
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, catSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &catSkillModel->catSkillButton, &catSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME || returnStateId == CHOOSE_MOUSE){ /* Done button was pressed */
		catSkillModel->isCatHuman = 0; /* cat us a machine */
		catSkillModel->catSkill = catSkillModel->currValueTemp; /* set cat skill once done is pressed */
		if (returnStateId == CHOOSE_MOUSE){ /* update pre choose mouse in model */
			catSkillModel->preChooseMouse = CAT_SKILL;
		}
	}
	else if (returnStateId == CHOOSE_CAT){ /* back was pressed and we go back to choose cat */
		catSkillModel->catSkillButton = 0;
		if (catSkillModel->preChooseCat == MAIN_MENU) /* we pressed back when we are not during game */
			catSkillModel->catSkill = DEFAULT_SKILL; /* set skill to deafult skill */
	}
	catSkillModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for mouse skill. Calls generalMenuPHE */
StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MOUSE_SKILL; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef mouseSkillModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId mouseSkillStates[COMMON_MENU_NUM_BUTTONS] = {MOUSE_SKILL, PLAY_GAME, CHOOSE_MOUSE};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mouseSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &mouseSkillModel->mouseSkillButton, &mouseSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME){ /* Done button was pressed */
		mouseSkillModel->isMouseHuman = 0; /* set mouse as machine */
		mouseSkillModel->mouseSkill = mouseSkillModel->currValueTemp;
	}
	else if (returnStateId == CHOOSE_MOUSE){ /* back was pressed */
		mouseSkillModel->mouseSkillButton = 0; /* set button to 0 for when we get back */
	}
	mouseSkillModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for load game menu. calls generalMenuPHE */
StateId loadGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = LOAD_GAME;/* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef loadGameModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId loadGameStates[COMMON_MENU_NUM_BUTTONS] = {LOAD_GAME, CHOOSE_CAT, MAIN_MENU};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, loadGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &loadGameModel->loadGameButton, &loadGameModel->loadGameWorld, MAX_WORLD);
	if (returnStateId == CHOOSE_CAT){ /* done was pressed */
		loadGameModel->preChooseCat = LOAD_GAME; /* update the pre choose cat stateId */
		loadGameModel->loadFromFile = 1; /* update load from file to 1 */
	}
	loadGameModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for edit game menu. calls generalMenuPHE */
StateId editGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = EDIT_GAME; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef editGameModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId editGameStates[COMMON_MENU_NUM_BUTTONS] = {EDIT_GAME, WORLD_BUILDER, MAIN_MENU};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, editGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &editGameModel->editGameButton, &editGameModel->editedWorld, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){ /* done was pressed */
		editGameModel->preWorldBuilder = EDIT_GAME; /* update pre world builder */
		editGameModel->loadFromFile = 1; /* set load from file to 1 */
	}
	editGameModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for save world menu. calls generalMenuPHE */
StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = SAVE_WORLD; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef saveWorldModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId saveWorldStates[COMMON_MENU_NUM_BUTTONS] = {SAVE_WORLD, WORLD_BUILDER, WORLD_BUILDER};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, saveWorldStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &saveWorldModel->saveWorldButton, &saveWorldModel->currValueTemp, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){ /* done or back were pressed */
		saveWorldModel->preWorldBuilder = SAVE_WORLD;
		if (saveWorldModel->saveWorldButton == 1){ /* done was pressed */
			saveWorldModel->loadFromFile = 1; /* tell wb to use file to load the grid */
			saveWorldModel->editedWorld = saveWorldModel->currValueTemp; /* set edited world to curr value */
			/* save the grid data to the correct file and free it */
			saveGameDataToFile(saveWorldModel->editedWorld, saveWorldModel->isCatFirst, saveWorldModel->gameGridData);
			freeGridData(saveWorldModel->gameGridData); /* free grid data */
			saveWorldModel->gameGridData = NULL;
		}
		else if (saveWorldModel->saveWorldButton == 2) /* back was pressed */
			saveWorldModel->loadFromFile = 0; /* tell wb to use char ** to load the grid */
	}
	saveWorldModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}
