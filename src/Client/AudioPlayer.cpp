#include "Client\AudioPlayer.h"

AudioPlayer::AudioPlayer(SoundBuffers & soundBuffers) :mSoundBuffers(soundBuffers)
{

}

void AudioPlayer::playMusic(const std::string & s)
{
	mMusics[s].openFromFile(s);
	mMusics[s].play();
}
void AudioPlayer::stopMusic(const std::string & s)
{
	mMusics[s].stop();
}
void AudioPlayer::playSound(const std::string & s)
{
	sf::Sound sound;
	sound.setBuffer(*mSoundBuffers.get(s));
	sound.play();
	mSounds.push_back(sound);
}
void AudioPlayer::update()
{
	auto isDone = [](const sf::Sound & s) -> bool
	{
		return s.getStatus() == sf::Sound::Stopped;
	};
	mSounds.remove_if(isDone);
}
