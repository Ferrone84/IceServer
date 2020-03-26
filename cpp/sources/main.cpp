#include <Ice/Ice.h>
#include <Server.h>
#include <signal.h>

using namespace std;
using namespace server;

//const string ip = "10.126.2.160";
//const string ip = "10.120.11.177"; //fac
const string ip = "192.168.1.32"; //maison
bool shutdown = false;

// attrape un signal
void catchStop(int s) {
	cout << "\nSignal d'extinction reçu, le serveur va se fermer." << endl;
	cout << "Appuyer sur entrée pour continuer." << endl;
	shutdown = true;
}

int main(int argc, char* argv[]) {
	signal(SIGINT, &catchStop); //cette fonction est appelée si le signal d'extinction SIGINT (ctrl+c) est reçu

	cout << "Lancement du serveur." << endl;
	try {
		Ice::CommunicatorHolder ich(argc, argv);
		auto adapter = ich->createObjectAdapterWithEndpoints("Serveur", "tcp -h " + ip + " -p 10000");
		auto servant = make_shared<Server>();
		adapter->add(servant, Ice::stringToIdentity("Serveur"));
		adapter->activate();
		cout << "En attente de réponse d'un client ...\n" << endl;

		while (!shutdown) //permet au serveur d'attendre une réponse sans consommer d'opération
			cin.get();

		ich->shutdown(); //si l'utilisateur ctrl+c, le serveur se coupe
	}
	catch(const exception& e) {
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}
