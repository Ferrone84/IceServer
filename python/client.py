#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, Ice

# J'ajoute le dossier parent au python path pour pouvoir utiliser le module server
currentdir = os.path.join(os.path.dirname(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir)

# Initialisation de la communication avec le serveur
import server

#ip = "192.168.1.32"
ip = "10.126.1.5"
def initServer():
	global communicator
	global server
	communicator = Ice.initialize(sys.argv)
	base = communicator.stringToProxy("Serveur:tcp -h " + ip + " -p 10000")
	try:
		server = server.IServerPrx.checkedCast(base)
	except:
		print("Veuillez lancer le serveur avant le client s'il vous plait.")
		communicator.destroy()
		exit()

	if not server:
		raise RuntimeError("Invalid proxy")


# Fonctions natives au serveur
def addMusic():
	print "Pour créer une musique dans la playlist, il faut : "
	print "Entrez le nom de la musique : "
	name = raw_input("> ")
	print "Entrez le nom de l'artiste : "
	artist = raw_input("> ")
	print "Entrez le nom de l'album : "
	album = raw_input("> ")
	server.addMusic(name, artist, album)
	print "La musique (" + name + ", " + artist + ", " + album + ") a bien été ajoutée à la playlist."

def addMusicPath():
	print "Pour créer une musique dans la playlist à partir d'un fichier mp3, il faut : "
	print "Entrez le nom du fichier (sans l'extension) : "
	name = raw_input("> ")

	result = server.addMusicPath(name);
	if (result):
		print "yep"
	else:
		print "nope"

def playMusic():
	playlist = server.getPlaylist()
	if (len(playlist) == 0):
		print "Vous ne pouvez pas jouer de musiques, car la playlist ne contient aucune musique."
		return
	print("Affichage du contenu de la playlist : ")
	print(playlist)
	print "Entrez le numéro de la musique que vous voulez jouer : "
	musicId = int(raw_input("> "))
	server.playMusic(musicId)

def stopMusic():
	stop = server.stopMusic();
	if (stop):
		print "La musique a bien été stoppée."
	else:
		print "Le stream n'est pas lancé."

def remove():
	print "Entrez le numéro de la musique que vous voulez enlever : "
	musicId = int(raw_input("> "))
	result = server.remove(musicId);
	if (result):
		print "La musique avec le numéro",musicId,"a bien été enlevée de la liste."
	else:
		print "Impossible de supprimer cette musique de la playlist, car le numéro",musicId," n'existe pas."

def findById():
	print "Entrez le numéro de la musique que vous recherchez : "
	musicId = int(raw_input("> "))
	music = server.findById(musicId)
	if (music.id == -1):
		print "Ce numéro de musique n'existe pas."
		return
	print "La musique avec le numéro",musicId,"a bien été récuppérée. La voici : "
	print(music)

def findByName():
	print "Entrez le nom des musiques que vous recherchez : "
	musicName = raw_input("> ")
	playlist = server.findByName(musicName)
	if (len(playlist) == 0):
		print "Aucune musique portant ce nom n'a été trouvée."
	else:
		print "Pour le nom '" + musicName + "' il existe " + str(len(playlist)) + " musiques. Les voici : "
		print(playlist)

def findByArtist():
	print "Entrez le nom de l'artiste des musiques que vous recherchez : "
	musicArtist = raw_input("> ")
	playlist = server.findByArtist(musicArtist)
	if (len(playlist) == 0):
		print "Aucune musique portant ce nom d'artiste n'a été trouvée."
	else:
		print "Pour le nom d'artiste '" + musicArtist + "' il existe " + str(len(playlist)) + " musiques. Les voici : "
		print(playlist)

def findByAlbum():
	print "Entrez le nom d'album des musiques que vous recherchez : "
	album = raw_input("> ")
	playlist = server.findByAlbum(album)
	if (len(playlist) == 0):
		print "Aucune musique de cet album n'a été trouvée."
	else:
		print "Pour l'album' '" + album + "' il existe " + str(len(playlist)) + " musiques. Les voici : "
		print(playlist)

def display():
	server.display() #fait aussi un affichage côté serveur
	playlist = server.getPlaylist()
	if (len(playlist) == 0):
		print "La playlist ne contient aucune musique."
		return
	print("Affichage du contenu de la playlist : ")
	print(playlist)


# Fonctions liées à l'affichage et la gestion des entrées utilisateur
def selectChoice(choices):
	stop = False
	while not stop:
		stop = True;
		choice = raw_input("\033[36;1m> \033[0m")

		if choice in choices:
			return choice
		else:
			stop = False
			print "Choix incorrect ! Vous devez chosir parmi : ",
			for c in choices:
				print c,
		print ""


def displayMainMenu():
	print "\033[36;1mMenu Principal -> Veuillez choisir un des choix suivants : \033[0m\n"
	print "\033[31;1m1 - \033[0m \033[32;1mAjouter une musique à la playlist\033[0m"
	print "\033[31;1m2 - \033[0m \033[32;1mJouer une musique de la playlist grâce à son numéro\033[0m"
	print "\033[31;1m3 - \033[0m \033[32;1mStoper la musique en cours\033[0m"
	print "\033[31;1m4 - \033[0m \033[32;1mRetirer une musique de la playlist grâce à son numéro\033[0m"
	print "\033[31;1m5 - \033[0m \033[32;1mRechercher une musique dans la playlist par son numéro\033[0m"
	print "\033[31;1m6 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur nom\033[0m"
	print "\033[31;1m7 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur nom d'artiste\033[0m"
	print "\033[31;1m8 - \033[0m \033[32;1mRechercher des musiques dans la playlist par leur nom d'album\033[0m"
	print "\033[31;1m9 - \033[0m \033[32;1mAfficher le contenu de la playlist\033[0m"
	print "\033[31;1m0 - \033[0m \033[32;1mSortir du programme\033[0m\n"

	choice = selectChoice(["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]) #Tous les choix possibles

	if choice == "0":
		print "\033[34;1mAu revoir et à bientôt ! :)\033[0m\n"
		if communicator:
			try:
				communicator.destroy()
			except:
				traceback.print_exc()
				status = 1
		sys.exit(0)

	os.system("clear")

	if choice == "1":
		print "Mano ou fichier ? 1/2"
		result = raw_input("> ")
		if (result == 1):
			addMusic()
		else:
			addMusicPath()

	elif choice == "2":
		playMusic()

	elif choice == "3":
		stopMusic()

	elif choice == "4":
		remove()

	elif choice == "5":
		findById()

	elif choice == "6":
		findByName()

	elif choice == "7":
		findByArtist()

	elif choice == "8":
		findByAlbum()

	elif choice == "9":
		display()

	raw_input("\n\033[36;1mAppuyer sur entree pour revenir au menu.\033[0m")
	os.system("clear")
	displayMainMenu()

# Fonctions principales
def Main():
	initServer()
	os.system("clear")
	print "\033[34;1mBienvenue sur l'application.\033[0m\n"
	try:
		displayMainMenu()
	except SystemExit as e:
		sys.exit(e)
	except:
		print "\n\033[31;1mUne erreur est survenue pendant l'execution de l'application.\033[0m\n"
		if communicator:
			try:
				communicator.destroy()
			except:
				traceback.print_exc()
				status = 1
		exit()


#Code global
try:
	Main()
except KeyboardInterrupt:
	print "\n\033[34;1mAu revoir et à bientôt ! :)\033[0m\n"
	if communicator:
		try:
			communicator.destroy()
		except:
			traceback.print_exc()
			status = 1
	exit()


