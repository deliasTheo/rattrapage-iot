# Rapport - Projet IoT ESP32

## Navigation Git

Chaque avancement valide du projet est repere par un tag Git. Pour consulter une etape precise, le correcteur peut utiliser `git checkout tp1-pilotage-local`, `git checkout tp1-5-json-serie`, `git checkout tp3-wifi-http-littlefs` ou `git checkout tp4-mqtt`.

## Avancement 1 - TP1 : Pilotage local de l'ESP

### Resume de la periode couverte par le tag

Cette periode couvre le demarrage du projet IoT et la realisation du TP1 : pilotage local de l'ESP32. L'objectif etait de produire une regulation locale de temperature sans utiliser WiFi, HTTP, MQTT, Node-RED ou JSON.

Le code correspondant se trouve dans `esp_iot/esp_iot.ino`. L'etat de cette etape est retrouve avec le tag Git `tp1-pilotage-local`.

### Travaux realises

- Consultation des consignes projet dans `AGENTS.md`.
- Consultation du document officiel `docs/TP1.0.pdf`, notamment la page 50, section 11.1.
- Identification des broches imposees par le TP : capteur de temperature sur `GPIO 23`, LED verte sur `GPIO 19`, LED rouge sur `GPIO 21`, ventilateur sur `GPIO 27`.
- Creation du sketch Arduino principal dans `esp_iot/esp_iot.ino`.
- Mise en place d'une regulation locale avec lecture de temperature, seuil bas, seuil haut, chauffage, climatisation et ventilateur.
- Ajout d'un filtrage simple par moyenne des 5 dernieres mesures.
- Ajout d'une hysteresis et d'un delai minimal entre deux changements d'etat.
- Ajout d'une option de PWM pour le ventilateur, desactivee par defaut tant que le montage moteur n'est pas confirme.
- Correction de l'organisation du projet pour garder un dossier de code unique evolutif.
- Validation de la partie TP1 et creation du tag Git `tp1-pilotage-local`.

### Decisions techniques importantes

La principale decision a ete de rester strictement dans le perimetre du TP1. Aucune communication reseau, aucun format JSON et aucune integration Node-RED ou MQTT n'ont ete ajoutes.

La regulation ne s'appuie pas sur une mesure instantanee. Une moyenne glissante de 5 mesures est utilisee pour reduire l'impact du bruit de mesure. Une hysteresis de `1.0 C` evite les basculements rapides autour des seuils. Un delai minimal de `5 s` limite aussi les changements d'etat trop frequents.

Le ventilateur est commande en tout-ou-rien par defaut. Une commande PWM existe dans le code, mais reste desactivee car le montage materiel n'a pas encore ete confirme.

L'organisation du projet a ete corrigee apres retour utilisateur. Le code n'est pas range dans un dossier par TP. Il reste dans un dossier unique, `esp_iot`, qui sera modifie progressivement. Les tags Git servent a retrouver les versions validees.

### Difficultes rencontrees

La premiere difficulte a ete materielle : l'ESP n'etait pas branche et le capteur DHT n'etait pas clairement identifie au depart.

Une autre difficulte concernait l'organisation du projet. Une premiere proposition avait cree une documentation separee pour TP1. Cette approche ne correspondait pas a la logique souhaitee. Elle a ete corrigee pour privilegier un dossier de code evolutif et une documentation centralisee dans le rapport.

La compilation automatique n'a pas pu etre faite depuis le terminal car `arduino-cli` n'etait pas installe. La validation finale repose donc sur la validation utilisateur et, idealement, une verification dans Arduino IDE.

### Analyse de la qualite des prompts

Les prompts etaient globalement clairs sur la progression attendue : commencer par TP1, ne pas sauter d'etape, et rester dans une logique de projet incrementale.

Le retour sur l'organisation du code a ete particulierement utile. Il a permis de corriger une mauvaise interpretation : l'objectif n'etait pas de creer un dossier par TP, mais de maintenir un code unique dont les etats sont retrouves par tags Git.

Les demandes sur le branchement etaient formulees de maniere concrete. Elles ont montre que l'accompagnement devait etre plus visuel et plus progressif pour la partie materielle.

### Mes points forts dans l'utilisation de l'IA

L'utilisateur a donne un contexte projet precis, avec des regles claires dans `AGENTS.md`. Cela a permis de cadrer le developpement et d'eviter de passer trop vite aux etapes suivantes.

L'utilisateur a aussi corrige rapidement une organisation qui ne lui convenait pas. Cette intervention a ameliore la structure du projet et a clarifie la methode de travail pour les prochaines etapes.

### Mes points d'amelioration

Il faudra continuer a valider explicitement l'organisation avant de creer plusieurs fichiers. Une proposition courte de structure doit etre faite avant toute modification importante.

Il faudra aussi distinguer plus clairement trois niveaux : le code, le journal de bord `notes.md`, et le rapport final `rapport.md`.

La documentation intermediaire ne doit pas etre dispersee dans des fichiers supplementaires sauf demande explicite.

### Situations ou nous avons perdu du temps ou rencontre des blocages

Du temps a ete perdu sur la premiere organisation des fichiers. La creation d'un dossier TP1 dedie et de fichiers de validation separes ne correspondait pas a la philosophie attendue. Cette erreur a ete corrigee avant la validation.

### Enseignements tires de cette etape

Cette etape montre l'interet d'une progression stricte : commencer par une regulation locale simple avant d'ajouter les communications.

Elle montre aussi que les tags Git sont utiles pedagogiquement. Ils permettent de retrouver un etat valide du code sans dupliquer les dossiers ou figer artificiellement chaque TP dans une arborescence differente.

Enfin, elle confirme qu'un projet IoT doit traiter le bruit de mesure des le depart. Une regulation basee sur une seule mesure instantanee serait trop instable.

### Autres observations pertinentes

Le code est volontairement simple pour rester compatible avec le niveau TP1. Il pourra etre restructure progressivement si les prochaines etapes l'exigent, mais sans introduire de complexite inutile trop tot.

### Notes personnelles

Pour l'instant, l'IA se debrouille plutot bien. J'ai passé pas mal de temps aussi avec chatgpt en dehors du projet pour essayer de voir ce qu'il me conseillé comme nom de fichier, pour le 'AGENTS.md' par exemple, ou ce qu'il fallais mettre dedans. A voir si pour le reste du projet ca se passe aussi bien.

## Avancement 2 - TP1.5 : Communication serie et JSON

### Resume de la periode couverte par le tag

Cette periode correspond au TP1.5 : communication serie et production d'un JSON. L'etat de cette etape est retrouve avec le tag Git `tp1-5-json-serie`.

L'objectif est de conserver la regulation locale du TP1 tout en ajoutant une communication serie structuree en JSON. Le JSON sera ensuite reutilisable dans les etapes suivantes.

### Travaux realises

- Consultation de `docs/TP1.5-json.pdf`, notamment la page 28 qui donne le modele JSON de reference.
- Consultation des pages 29 a 33, qui presentent ArduinoJson et l'interet de valider les donnees JSON.
- Ajout de la bibliotheque ArduinoJson dans le sketch.
- Extension de `esp_iot/esp_iot.ino` sans remplacer la regulation TP1.
- Production d'un JSON sur le port serie a chaque mesure.
- Ajout des sections principales du modele : `status`, `location`, `regul`, `info`, `net` et `reporthost`.
- Remplacement du message texte d'erreur DHT par un objet JSON contenant `status.error`.
- Suppression des dossiers `samples/` et `tools/` apres retour utilisateur, car ils ajoutaient un outillage non demande.

### Decisions techniques importantes

Le code TP1 n'a pas ete remplace. Il a ete etendu pour ajouter la sortie JSON tout en conservant la regulation locale, le filtrage, l'hysteresis et la commande des sorties.

Le JSON est emis sur une seule ligne a chaque mesure. Ce choix facilite la lecture dans le moniteur serie et la reutilisation future dans Node-RED ou dans un autre outil.

Les champs directement disponibles sont remplis depuis le programme : temperature filtree, temperature brute, etat chauffage, etat climatisation, vitesse du ventilateur, seuil bas et seuil haut.

Les champs reseau restent volontairement neutres (`NOP` ou `0.0.0.0`), car le WiFi n'a pas encore ete implemente. Ce choix evite de simuler une etape qui n'a pas encore ete validee.

### Difficultes rencontrees

Une erreur de perimetre a ete faite au debut de l'etape : des dossiers `samples/` et `tools/` avaient ete ajoutes pour proposer un exemple JSON et un validateur local. Ces outils pouvaient etre utiles, mais ils n'etaient pas demandes. Ils ont donc ete supprimes.

La compilation automatique n'a toujours pas ete faite depuis le terminal car `arduino-cli` n'est pas disponible. La validation devra donc passer par Arduino IDE.

### Analyse de la qualite des prompts

La demande initiale etait claire : passer a l'etape 2, continuer a ecrire dans `notes.md`, mais ne pas completer le rapport tout de suite.

Le retour utilisateur a precise que les dossiers `samples/` et `tools/` ne correspondaient pas a la demande. Cette remarque a permis de recentrer l'etape sur le code ESP uniquement.

### Mes points forts dans l'utilisation de l'IA

L'utilisateur a su limiter le perimetre de la demande et corriger rapidement une proposition trop large.

L'utilisateur a aussi indique clairement quand une information ne devait pas etre ajoutee au rapport ou aux notes, puis quand elle pouvait l'etre. Cela a permis de mieux controler la trace ecrite du projet.

### Mes points d'amelioration

Il faut eviter d'ajouter des outils annexes, meme utiles, sans demander confirmation. Dans ce projet, chaque ajout doit correspondre directement a l'etape en cours ou a une demande explicite.

Il faut aussi mieux separer ce qui est necessaire pour le TP de ce qui est seulement une suggestion d'amelioration.

### Situations ou nous avons perdu du temps ou rencontre des blocages

Un petit blocage est apparu avec l'ajout du validateur et de l'exemple JSON. La correction a consiste a supprimer ces fichiers et a garder uniquement le code ESP et les notes.

### Enseignements tires de cette etape

Le debut du TP1.5 montre l'importance d'un format d'echange stable. Le modele JSON impose par le cours evite que chaque objet connecte produise une structure differente.

Cette etape montre aussi qu'il faut respecter strictement le perimetre d'une demande. Ajouter un outil utile peut etre contre-productif si cela complique le projet ou s'eloigne de l'objectif immediat.

### Autres observations pertinentes

Pour valider TP1.5, il faudra compiler le sketch dans Arduino IDE avec les bibliotheques `DHT sensor library` et `ArduinoJson`, puis verifier que le moniteur serie affiche une ligne JSON valide a chaque mesure.

### Notes personnelles

Cette partie m'a permis de voir que le JSON n'est pas seulement un format d'affichage, mais une maniere de structurer proprement les donnees pour la suite du projet. J'ai aussi vu qu'il fallait faire attention a ne pas accepter automatiquement tout ce que l'IA propose : certains ajouts peuvent etre utiles en theorie, mais ne pas correspondre exactement a ce qui est demande pour le TP. Le fait d'avoir supprime `samples/` et `tools/` montre que je dois garder le controle sur le perimetre du projet.

## Avancement 3 - TP1.75 / TP2.5 / TP3.0 : WiFi, HTTP et page HTML en Flash

### Resume de la periode couverte par le tag

Cette periode correspond a la partie 3 : connexion WiFi, serveur HTTP et page HTML stockee dans la memoire Flash de l'ESP32 avec LittleFS.

L'etat de cette etape est retrouve avec le tag Git `tp3-wifi-http-littlefs`. L'objectif etait de verifier que l'ESP pouvait quitter la simple communication serie pour devenir accessible depuis un navigateur sur le reseau local.

### Travaux realises

- Consultation de `docs/TP1.75.pdf` pour LittleFS et le stockage de fichiers HTML dans la memoire Flash.
- Consultation de `docs/TP2.5-wifi.pdf` pour la connexion WiFi de l'ESP32 en mode station.
- Consultation de `docs/TP3.0-http.pdf` pour la mise en place d'un serveur HTTP sur l'ESP.
- Ajout de `WiFi.h`, `WebServer.h` et `LittleFS.h` dans le sketch principal.
- Ajout de constantes `WIFI_SSID`, `WIFI_PASSWORD` et `WIFI_HOSTNAME`.
- Ajout d'un serveur HTTP sur le port `80`.
- Ajout des routes `/`, `/status`, `/temperature`, `/leds`, `/fan` et `/set`.
- Creation du dossier `esp_iot/data/` avec `index.html` et `esp.css`, destines a etre uploades dans LittleFS.
- Conservation de la regulation locale TP1 et de la sortie JSON serie TP1.5.
- Ajout d'une interface HTML simple pour afficher temperature, etat des LEDs, etat du ventilateur, reseau et seuils.

### Decisions techniques importantes

Le choix principal a ete d'utiliser `WebServer` avec `WiFi.h` et `LittleFS`, plutot que `ESPAsyncWebServer`. Le cours presente le serveur asynchrone, mais cette version simple limite les dependances externes et reste plus facile a tester progressivement.

La page HTML n'est pas integree directement comme grande chaine de caracteres dans le fichier `.ino`. Elle est placee dans `esp_iot/data/index.html`, avec une feuille de style `esp_iot/data/esp.css`, afin de respecter l'idee du TP : utiliser la memoire Flash de l'ESP pour stocker des fichiers Web.

La route `/status` reutilise la structure JSON deja construite lors de TP1.5. Cela garde une coherence entre la communication serie et la communication HTTP.

Les seuils `seuilBas` et `seuilHaut` sont devenus modifiables depuis la route `/set`, ce qui permet a la page HTML d'agir sur la regulation sans ajouter MQTT ou Node-RED.

### Difficultes rencontrees

Une erreur de code a ete reperee apres la modification : une ancienne constante `SEUIL_HAUT` etait encore utilisee alors que les seuils avaient ete transformes en variables modifiables. Elle a ete remplacee par `seuilHaut`.

La partie LittleFS ajoute une contrainte de validation supplementaire : il ne suffit plus de televerser le firmware, il faut aussi uploader le dossier `data` dans la memoire Flash. Si cette etape est oubliee, la route `/` ne pourra pas afficher `index.html`.

La compilation automatique n'a toujours pas ete faite depuis le terminal car `arduino-cli` n'est pas disponible.

### Analyse de la qualite des prompts

La demande etait courte mais claire : passer a la partie 3. Le contexte du projet et les consignes dans `AGENTS.md` ont permis d'identifier les documents a lire et de ne pas commencer MQTT trop tot.

Le prompt laisse une marge de decision technique. Cette marge a ete utilisee pour choisir une architecture HTTP simple, plus adaptee a une progression etape par etape.

### Mes points forts dans l'utilisation de l'IA

Le projet avance avec une bonne logique de validation progressive. Les etapes precedentes ont ete taguees avant de commencer la suivante, ce qui permet de revenir facilement en arriere.

Les corrections faites lors des etapes precedentes ont aussi aide a mieux cadrer cette partie : garder un seul dossier de code, eviter les fichiers annexes non demandes, et documenter les choix dans `notes.md`.

### Mes points d'amelioration

Il faudra tester plus systematiquement la compilation dans Arduino IDE, car l'absence de `arduino-cli` limite la verification automatique.

Il faudra aussi faire attention aux identifiants WiFi. Ils ne doivent pas etre commits s'ils correspondent a un reseau personnel sensible. Pour l'instant, le code contient seulement des valeurs placeholder.

### Situations ou nous avons perdu du temps ou rencontre des blocages

Le principal risque de perte de temps concerne LittleFS : il faut bien uploader `esp_iot/data/` avant de tester la page Web. Sans cela, le serveur HTTP peut fonctionner mais la page HTML sera introuvable.

Un autre point de vigilance est le choix entre serveur HTTP simple et serveur asynchrone. La version simple est plus pragmatique pour l'instant, mais il faudra accepter de la faire evoluer si les prochains besoins l'exigent.

### Enseignements tires de cette etape

Cette etape montre que l'ESP devient progressivement un objet connecte complet : il ne se contente plus d'envoyer des donnees sur le port serie, il expose aussi une interface consultable depuis un navigateur.

Elle montre aussi l'interet de separer le code embarque et les fichiers Web. Le fichier `.ino` reste concentre sur les capteurs, la regulation et les routes HTTP, tandis que l'interface utilisateur est dans `data/index.html`.

### Autres observations pertinentes

Pour valider cette partie, il faudra remplacer `WIFI_SSID` et `WIFI_PASSWORD`, uploader LittleFS, uploader le firmware, recuperer l'adresse IP de l'ESP, puis tester `/`, `/status`, `/temperature`, `/leds` et `/fan` dans un navigateur.

Cette partie a ete taguee avant de commencer MQTT, afin de conserver un point de retour clair.

### Notes personnelles

Cette partie me semble plus concrete parce qu'on commence a voir l'ESP comme un vrai objet connecte accessible depuis un navigateur. J'ai aussi compris qu'une etape comme LittleFS peut etre piegeuse : le code peut etre bon, mais si les fichiers HTML ne sont pas uploades dans la Flash, rien ne marche comme prevu. Le choix d'une version HTTP simple me parait coherent pour avancer sans ajouter trop de dependances d'un coup.

## Avancement 4 - TP4.2 : MQTT

### Resume de la periode couverte par le tag

Cette periode correspond a la derniere partie du projet : ajout du protocole MQTT. L'etat de cette etape est retrouve avec le tag Git `tp4-mqtt`.

L'objectif est de faire evoluer l'ESP vers un objet qui publie ses informations sur un broker MQTT et qui peut recevoir quelques commandes via un topic de souscription, tout en conservant les etapes precedentes : regulation locale, JSON serie, WiFi, HTTP et page HTML.

### Travaux realises

- Consultation de `docs/TP4-mqtt.pdf`.
- Identification de la bibliotheque recommandee : `PubSubClient`.
- Ajout de `PubSubClient.h` dans le sketch principal.
- Configuration du broker `test.mosquitto.org` sur le port `1883`.
- Publication de l'etat JSON sur le topic `uca/iot/master`, conformement au TODO du document MQTT.
- Souscription au topic `uca/iot/master/commands`.
- Ajout d'une commande JSON pour modifier les seuils `lt` et `ht`.
- Ajout d'une commande `publish_now` pour forcer une publication MQTT immediate.
- Augmentation du buffer MQTT a `2048` octets pour eviter que le JSON soit trop grand pour la taille standard.
- Ajout de l'etat MQTT dans le JSON `/status`, dans la route HTTP `/mqtt`, et dans la page HTML.

### Decisions techniques importantes

La decision principale a ete d'utiliser `PubSubClient`, comme conseille dans le document du cours. Cette bibliotheque est synchrone et reste coherente avec l'architecture simple deja choisie pour HTTP.

Le topic de publication est `uca/iot/master`, car le document MQTT demande de publier sur ce topic le meme JSON que celui utilise pour le reporting. Cela permet de reutiliser la structure JSON mise en place lors de TP1.5 et exposee ensuite par HTTP.

Un topic de commandes separe, `uca/iot/master/commands`, a ete ajoute pour eviter de melanger les messages d'etat publies par l'ESP et les commandes envoyees vers l'ESP.

Les commandes MQTT restent volontairement limitees. Elles permettent de modifier les seuils et de demander une publication immediate. Cela montre la reception de commandes sans introduire une logique applicative trop complexe.

### Difficultes rencontrees

La principale difficulte est que MQTT depend completement de la connexion WiFi. Tant que `WIFI_SSID` et `WIFI_PASSWORD` ne sont pas remplaces par des valeurs valides, l'ESP ne peut pas joindre le broker.

Une autre difficulte concerne la taille du JSON. Le cours signale que `PubSubClient` a une taille de payload limitee par defaut. Le buffer a donc ete augmente a `2048` octets.

La compilation automatique n'a toujours pas ete faite depuis le terminal car `arduino-cli` n'est pas disponible. La validation finale doit passer par Arduino IDE et par un client MQTT ou Node-RED.

### Analyse de la qualite des prompts

La demande etait claire sur l'ordre des operations : publier d'abord le tag de la partie 3, puis passer a la derniere partie, mettre a jour le rapport, ajouter une note personnelle, et creer un tag final.

Cette consigne a permis de respecter la logique de progression du projet et d'eviter d'ecraser l'etat WiFi/HTTP avant d'ajouter MQTT.

### Mes points forts dans l'utilisation de l'IA

L'utilisation des tags devient plus rigoureuse. Chaque etape importante peut etre retrouvee dans Git, ce qui facilite la redaction du rapport et la demonstration de la progression.

Le projet montre aussi une bonne reutilisation du code precedent : le JSON construit pour la serie et HTTP sert maintenant de payload MQTT.

### Mes points d'amelioration

Il faudra tester l'ensemble avec un vrai broker MQTT et un client de souscription. Le code est prepare, mais la validation pratique reste indispensable.

Il faudra aussi eviter de laisser des identifiants WiFi personnels dans le code avant un push public. Le projet garde actuellement des placeholders.

### Situations ou nous avons perdu du temps ou rencontre des blocages

Le principal risque de blocage est la configuration reseau : WiFi 2.4 GHz, acces Internet ou acces au broker, port `1883` ouvert, et bibliotheque `PubSubClient` installee.

Un autre risque est la confusion entre les topics. Le topic `uca/iot/master` sert a publier l'etat, tandis que `uca/iot/master/commands` sert aux commandes.

### Enseignements tires de cette etape

Cette etape montre la difference entre HTTP et MQTT. Avec HTTP, le navigateur vient interroger l'ESP. Avec MQTT, l'ESP publie ses donnees a un broker et d'autres clients peuvent les recevoir.

Elle montre aussi l'interet d'avoir conserve un JSON stable. La meme representation de l'etat de l'objet peut etre utilisee sur le port serie, sur HTTP et sur MQTT.

### Autres observations pertinentes

Pour valider cette partie, il faudra installer `PubSubClient`, compiler le sketch, connecter l'ESP au WiFi, puis ecouter le topic :

```bash
mosquitto_sub -h test.mosquitto.org -t uca/iot/master -v
```

Pour tester une commande :

```bash
mosquitto_pub -h test.mosquitto.org -t uca/iot/master/commands -m "{\"lt\":21,\"ht\":27,\"publish_now\":true}"
```

### Notes personnelles

Cette derniere partie montre bien pourquoi on a avance progressivement. Le JSON prepare avant sert maintenant directement pour MQTT, donc les choix faits dans les premieres etapes ont un vrai impact sur la suite. Je vois aussi que l'IA peut accelerer le developpement, mais que je dois rester attentif aux details pratiques : bibliotheques a installer, taille du payload MQTT, topics utilises, WiFi configure correctement et tests reels avec un client MQTT.
