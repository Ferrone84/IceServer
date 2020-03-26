import java.util.*;
import java.io.*;

public class Client
{
	private server.IServerPrx serverObject;

	public Client(server.IServerPrx serverObject) {
		this.serverObject = serverObject;
	}

	public void addMusic() {
		Scanner sc = new Scanner(System.in);
		System.out.println("Pour créer une musique dans la playlist, il faut : ");
		System.out.print("Entrez le nom de la musique : \n> ");
		String name = sc.nextLine();
		System.out.print("Entrez le nom de l'artiste : \n> ");
		String artist = sc.nextLine();
		System.out.print("Entrez la durée de la musique : \n> ");
		float time = sc.nextFloat();
		serverObject.addMusic(name, artist, time);
		System.out.println("La musique ("+ name + ", " + artist + ", " + time + ") a bien été ajoutée à la playlist.");
	}

	public void playMusic() {
		Scanner sc = new Scanner(System.in);
		server.Music[] playlist = serverObject.getPlaylist();
		if (playlist.length == 0) {
			System.out.println("Vous ne pouvez pas jouer de musiques, car la playlist ne contient aucune musique.");
			return;
		}
		System.out.println("Affichage du contenu de la playlist : \n");
		for (server.Music music : playlist)
			displayMusic(music);
		System.out.print("\nEntrez le numéro de la musique que vous voulez jouer : \n> ");
		int musicId = sc.nextInt();
		serverObject.playMusic(musicId);
	}

	public void remove() {
		Scanner sc = new Scanner(System.in);
		System.out.print("Entrez le numéro de la musique que vous voulez enlever : \n> ");
		int musicId = sc.nextInt();
		boolean result = serverObject.remove(musicId);
		if (result)
			System.out.println("La musique avec le numéro " + musicId + " a bien été enlevée de la liste.");
		else
			System.out.println("Impossible de supprimer cette musique de la playlist, car le numéro " + musicId + " n'existe pas.");
	}

	public void findById() {
		Scanner sc = new Scanner(System.in);
		System.out.print("Entrez le numéro de la musique que vous recherchez : \n> ");
		int musicId = sc.nextInt();
		server.Music music = serverObject.findById(musicId);
		if (music.id == -1) {
			System.out.println("Ce numero de musique n'existe pas.");
			return;
		}
		System.out.println("La musique avec le numéro " + musicId + " a bien été récuppérée. La voici : ");
		displayMusic(music);
	}

	public void findByName() {
		Scanner sc = new Scanner(System.in);
		System.out.print("Entrez le nom des musiques que vous recherchez : \n> ");
		String musicName = sc.nextLine();
		server.Music[] playlist = serverObject.findByName(musicName);
		if (playlist.length == 0)
			System.out.println("Aucune musique portant ce nom n'a été trouvée.");
		else {
			System.out.println("Pour le nom '" + musicName + "' il existe " + playlist.length + " musiques. Les voici : ");
			for (server.Music music : playlist)
				displayMusic(music);
		}
	}

	public void findByArtist() {
		Scanner sc = new Scanner(System.in);
		System.out.print("Entrez le nom de l'artiste des musiques que vous recherchez : \n> ");
		String musicArtist = sc.nextLine();
		server.Music[] playlist = serverObject.findByArtist(musicArtist);
		if (playlist.length == 0)
			System.out.println("Aucune musique portant ce nom d'artiste n'a été trouvée.");
		else {
			System.out.println("Pour le nom d'artiste '" + musicArtist + "' il existe " + playlist.length + " musiques. Les voici : ");
			for (server.Music music : playlist)
				displayMusic(music);
		}
	}

	public void findByTime() {
		Scanner sc = new Scanner(System.in);
		System.out.print("Entrez la durée des musiques que vous recherchez : \n> ");
		float musicTime = sc.nextFloat();
		server.Music[] playlist = serverObject.findByTime(musicTime);
		if (playlist.length == 0)
			System.out.println("Aucune musique de cette durée n'a été trouvée.");
		else
			System.out.println("Pour le temps '" + musicTime + "' il existe " + playlist.length + " musiques. Les voici : ");
			for (server.Music music : playlist)
				displayMusic(music);
	}

	public void display() {
		serverObject.display(); //fait aussi un affichage côté serveur
		server.Music[] playlist = serverObject.getPlaylist();
		if (playlist.length == 0) {
			System.out.println("La playlist ne contient aucune musique.");
			return;
		}
		System.out.println("Affichage du contenu de la playlist : \n");
		for (server.Music music : playlist)
			displayMusic(music);
	}

	public void displayMusic(server.Music music) {
		System.out.println("Musique n°" + music.id + " : nom[" + music.name + "] artiste[" + music.artist + "] durée[" + music.time + "].");
	}

	public void clear() {
		System.out.print("\033[H\033[2J");  
		System.out.flush();
	}

	public void start() {
		clear();
		System.out.println("\033[34;1mBienvenue sur l'application.\033[0m\n");
		boolean stop = false;

		while (!stop) {
			System.out.println("\033[36;1mMenu Principal -> Veuillez choisir un des choix suivants : \033[0m\n");
			System.out.println("\033[31;1m1 - \033[0m \033[32;1mAjouter une musique à la playlist\033[0m");
			System.out.println("\033[31;1m2 - \033[0m \033[32;1mJouer une musique de la playlist grâce à son numéro\033[0m");
			System.out.println("\033[31;1m3 - \033[0m \033[32;1mRetirer une musique de la playlist grâce à son numéro\033[0m");
			System.out.println("\033[31;1m4 - \033[0m \033[32;1mRechercher une musique dans la playlist par son numéro\033[0m");
			System.out.println("\033[31;1m5 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur nom\033[0m");
			System.out.println("\033[31;1m6 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur nom d'artiste\033[0m");
			System.out.println("\033[31;1m7 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur durée\033[0m");
			System.out.println("\033[31;1m8 - \033[0m \033[32;1mAfficher le contenu de la playlist\033[0m");
			System.out.println("\033[31;1m0 - \033[0m \033[32;1mSortir du programme\033[0m\n");

			System.out.print("> ");

			String[] choices = new String[] {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

			Scanner sc = new Scanner(System.in);
			String choice = sc.nextLine();

			while (! Arrays.asList(choices).contains(choice)) {
				System.out.print("Choix incorrect ! Vous devez chosir parmi : ");
				for (String c : choices)
					System.out.print(c + " ");
				System.out.print("\n> ");
				choice = sc.nextLine();
			}

			if (choice.equals("0")) {
				System.out.println("\n\033[34;1mAu revoir et à bientôt ! :)\033[0m\n");
				stop = true;
			}
			else {
				clear();
				if (choice.equals("1"))
					addMusic();
				else if (choice.equals("2"))
					playMusic();
				else if (choice.equals("3"))
					remove();
				else if (choice.equals("4"))
					findById();
				else if (choice.equals("5"))
					findByName();
				else if (choice.equals("6"))
					findByArtist();
				else if (choice.equals("7"))
					findByTime();
				else if (choice.equals("8"))
					display();
				System.out.print("\n\033[36;1mAppuyer sur entrée pour revenir au menu.\033[0m");
				sc.nextLine();
				clear();
			}
		}
	}

	public static void main(String[] args)
	{
		try (com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args))
		{
			com.zeroc.Ice.ObjectPrx base = communicator.stringToProxy("Serveur:default -p 10000");
			server.IServerPrx serverObject = server.IServerPrx.checkedCast(base);
			if (serverObject == null) {
				throw new Error("Invalid proxy");
			}
			Client client = new Client(serverObject);
			client.start();
		}
		catch (com.zeroc.Ice.ConnectionRefusedException e) {
			System.out.println("Veuillez lancer le serveur avant le client s'il vous plait.");
		}
		catch (Exception e) {
			System.out.println("\n\033[31;1mUne erreur est survenue pendant l'execution de l'application.\033[0m\n");
		}
	}  
}