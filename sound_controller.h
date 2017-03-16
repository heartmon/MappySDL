#pragma once
#include <SDL_mixer.h>
#include "game_entity.h"

class SoundController : public GameEntity {
  //system
	AvancezLib* system;

  //The music that will be played
  Mix_Music *music = NULL;

  //The sound effects that will be used
  Mix_Chunk *START_SOUND = NULL;
  Mix_Chunk *JUMP = NULL;
  Mix_Chunk *COLLECT_ITEM = NULL;
  Mix_Chunk *MICROWAVE = NULL;
  Mix_Chunk *DIE = NULL;
  Mix_Chunk *GAME_OVER_SOUND = NULL;
  Mix_Chunk *LEVEL_CLEAR_SOUND = NULL;

public:
	void Create(AvancezLib* system) {
		SDL_Log("SoundController::Create");
		//initialize
		loadMedia();
	}

	void Init() {
		GameEntity::Init();
	}

	void Destroy() {
		//Free the sound effects
		Mix_FreeChunk(START_SOUND);
		Mix_FreeChunk(JUMP);
		Mix_FreeChunk(COLLECT_ITEM);
		Mix_FreeChunk(MICROWAVE);
		Mix_FreeChunk(DIE);
		Mix_FreeChunk(GAME_OVER_SOUND);
		Mix_FreeChunk(LEVEL_CLEAR_SOUND);

		//Free the music
		Mix_FreeMusic(music);
		music = NULL;

		START_SOUND = NULL;
		JUMP = NULL;
		COLLECT_ITEM = NULL;
		MICROWAVE = NULL;
		DIE = NULL;
		GAME_OVER_SOUND = NULL;
		LEVEL_CLEAR_SOUND = NULL;

		//Quit SDL subsystems
		Mix_Quit();
	}

	void Update(float dt) {

	}

	void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == GAME_INIT) {
			Mix_PlayChannel(1, START_SOUND, 0);
			//void(*channel_finished)(int) = &ddd;
			//Mix_ChannelFinished(channel_finished);
			PlayMusic();
		}
		if (m->getMessageType() == MOUSE_JUMP_ON_ROPE) {
			Mix_PlayChannel(-1, JUMP, 0);
		}
		if (m->getMessageType() == IS_COLLECTED) {
			Mix_PlayChannel(-1, COLLECT_ITEM, 0);
		}
		if (m->getMessageType() == RELEASE_RAINBOW) {
			Mix_PlayChannel(-1, MICROWAVE, 0);
		}
		if (m->getMessageType() == MOUSE_DIE) {
			Mix_PlayChannel(-1, DIE, 0);
			StopMusic();
		}
		if (m->getMessageType() == GAME_OVER) {
			Mix_PlayChannel(-1, GAME_OVER_SOUND, 0);
		}
		if (m->getMessageType() == LEVEL_CLEAR) {
			Mix_PlayChannel(-1, LEVEL_CLEAR_SOUND, 0);
			StopMusic();
		}
		if (m->getMessageType() == ROUND_INIT) {
			StopMusic();
			PlayMusic();
		}

		


	}



private:
	//static void ddd(int channel) {
	//	SDL_Log("channel %d finished playback.\n", channel);
	//}

	void PlayMusic() {
		//If there is no music playing
		if (Mix_PlayingMusic() == 0)
		{
			//Play the music
			Mix_PlayMusic(music, -1);
		}
	}

	void StopMusic() {
		Mix_HaltMusic();
	}

	bool loadMedia() {
		//Loading success flag
		bool success = true;

		/*int flags = MIX_INIT_MP3;
		if (flags != (success = Mix_Init(flags))) {
			printf("Could not initialize mixer (result: %d).\n", success);
			printf("Mix_Init: %s\n", Mix_GetError());
		}*/
		//Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}

		
		//Load music
		music = Mix_LoadMUS("data/04-music-main-theme.wav");
		if (music == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}

		//Load sound effects
		START_SOUND = Mix_LoadWAV("data/03-game-start-sound.wav");
		if (START_SOUND == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		JUMP = Mix_LoadWAV("data/05-trampoline-jump.wav");
		if (JUMP == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		COLLECT_ITEM = Mix_LoadWAV("data/08-target-get.wav");
		if (COLLECT_ITEM == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		MICROWAVE = Mix_LoadWAV("data/07-microwave-take-off.wav");
		if (MICROWAVE == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		DIE = Mix_LoadWAV("data/19-miss.wav");
		if (DIE == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		GAME_OVER_SOUND = Mix_LoadWAV("data/22-game-over.wav");
		if (GAME_OVER_SOUND == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		LEVEL_CLEAR_SOUND = Mix_LoadWAV("data/09-round-clear-music.wav");
		if (LEVEL_CLEAR_SOUND == NULL) {
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}

		if(success) SDL_Log("Load sound succeeded");

		return success;
	}
};