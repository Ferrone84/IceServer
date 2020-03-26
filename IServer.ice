module server
{
	struct Music
	{
		int id;
		string name;
		string artist;
		string album;
		string path;
	};

	sequence<Music> playlist;

	interface IServer
	{
		void addMusic(string name, string artist, string album);
		bool addMusicPath(string name);
		void playMusic(int id);
		bool stopMusic();
		bool remove(int id);
		Music findById(int id);
		playlist findByName(string name);
		playlist findByArtist(string artist);
		playlist findByAlbum(string album);
		void display();
		playlist getPlaylist();
	};
};
