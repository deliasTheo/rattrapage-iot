# Journal de bord

## 2026-07-11 - Démarrage du TP1, pilotage local

La demande initiale était claire sur la progression souhaitée : commencer par le TP1 et avancer étape par étape, sans sauter vers les communications réseau. Les consignes du projet et le document `docs/TP1.0.pdf` ont été consultés avant de coder.

La section 11.1 du TP1.0 impose les broches suivantes : capteur de température sur GPIO 23, LED verte de climatisation sur GPIO 19, LED rouge de chauffage sur GPIO 21, ventilateur sur GPIO 27. Ces broches sont reprises dans le code pour rester compatible avec l'évaluation.

Décision technique importante : la première version reste totalement locale. Elle n'utilise pas encore JSON, WiFi, HTTP, MQTT ou Node-RED. La régulation intègre déjà une moyenne des 5 dernières mesures, une hystérésis et un délai minimal avant changement d'état pour tenir compte du bruit de mesure.

Point d'attention matériel : le ventilateur ne doit pas être alimenté directement par une broche GPIO de l'ESP32. Il faut utiliser un module adapté, un transistor/MOSFET ou commencer par tester la logique avec une LED avant de brancher le moteur.

## 2026-07-12 - Finalisation code de la partie TP1 locale

La question posee etait de savoir si la partie 1 etait terminee. Analyse : le code couvrait deja la regulation locale principale, mais il manquait une fiche de validation explicite et une clarification sur le ventilateur. Le code a donc ete complete sans passer a l'etape suivante.

Decision technique : le ventilateur reste commande en tout-ou-rien par defaut pour ne pas supposer un montage moteur compatible PWM. Une option `ACTIVER_PWM_VENTILATEUR` a ete ajoutee pour montrer la possibilite d'une vitesse progressive, comme apprecie dans le sujet, mais elle reste desactivee tant que le materiel n'est pas confirme.

Limite a mentionner dans le rapport : la partie est terminee cote code, mais pas encore validee physiquement sur l'ESP local. La validation finale doit idealement passer par une compilation Arduino IDE et, si possible, un essai sur carte.

## 2026-07-12 - Correction de l'organisation du projet

L'utilisateur a clarifie une attente importante : le code ne doit pas etre range dans un dossier different pour chaque TP. Le projet doit avoir un dossier de code unique, modifie progressivement a chaque etape. Les tags Git serviront a retrouver l'etat exact du code pour TP1, TP1.5, HTTP, MQTT, etc.

Correction appliquee : suppression des fichiers documentaires specifiques a TP1 (`README.md` et `VALIDATION_TP1.md`) et deplacement du sketch dans `esp_iot/esp_iot.ino`, qui devient le dossier principal du code ESP. Cette organisation correspond mieux a une progression incrementale et evite de figer chaque TP dans un dossier separe.

Decision pedagogique : les validations et explications detaillees ne doivent pas etre dispersees dans des fichiers de validation par TP. Elles seront conservees dans `notes.md` pendant le developpement, puis structurees dans `rapport.md` au moment ou une fonctionnalite sera validee et taguee.

## 2026-07-12 - Validation de la partie TP1

L'utilisateur a valide la partie TP1. Cette validation marque la fin de la premiere etape : regulation locale de temperature sur ESP32, sans communication reseau et sans JSON. Le code principal reste dans `esp_iot/esp_iot.ino`.

Le tag Git doit permettre de retrouver cet etat exact du projet pour le rapport et pour comparaison avec les etapes suivantes. Le rapport correspondant est genere a partir du journal de bord, conformement aux consignes du projet.

Point important pour le rapport : l'organisation du projet a ete ajustee apres retour utilisateur. La premiere proposition separait trop la documentation de TP1 dans des fichiers specifiques. La correction a permis d'adopter une logique plus propre : un code evolutif unique et des tags pour figer chaque etape validee.

## 2026-07-12 - Debut TP1.5, communication serie et JSON

L'etape 2 commence apres validation et tag du TP1. Les documents consultes sont `docs/TP1.5-json.pdf`, en particulier la page 28 qui donne le modele JSON attendu, et les pages 29 a 33 qui indiquent l'utilisation d'ArduinoJson et l'interet de valider le JSON produit.

Decision technique : le code TP1 n'est pas remplace. Il est etendu dans `esp_iot/esp_iot.ino` pour conserver la regulation locale et ajouter une sortie JSON sur le port serie. Le JSON reprend les grandes sections du modele fourni : `status`, `location`, `regul`, `info`, `net` et `reporthost`.

Le JSON est emis sur une seule ligne a chaque mesure. Ce choix facilite la copie depuis le moniteur serie et la reutilisation future dans Node-RED ou un script de validation. Les champs reseau restent des valeurs neutres (`NOP`, `0.0.0.0`) car le WiFi n'est pas encore une etape validee.

Correction ajoutee pendant l'etape : le sketch ne doit pas melanger texte libre et JSON sur le port serie. En cas d'erreur de lecture DHT, il emet maintenant aussi un objet JSON avec `status.error`, afin de garder un flux serie coherent pour la validation et les futures integrations.

Correction de perimetre apres retour utilisateur : les dossiers `samples/` et `tools/` ont ete supprimes, car ils ajoutaient un validateur et un exemple hors de la demande immediate. Le TP1.5 doit rester centre sur le code ESP qui produit un JSON propre sur le port serie. Cette erreur est interessante pour le rapport, car elle montre une limite de l'IA : tendance a ajouter des outils utiles mais non demandes.

## 2026-07-12 - Mise a jour du rapport pendant TP1.5

Le rapport a ete mis a jour a la demande de l'utilisateur pour inclure l'avancement TP1.5 et la correction de perimetre liee a la suppression de `samples/` et `tools/`. Au moment de cette note, aucun tag TP1.5 n'avait encore ete cree ; il a ensuite ete cree sous le nom `tp1-5-json-serie`.

## 2026-07-12 - Restructuration du rapport par avancement

L'utilisateur a precise que le rapport ne devait pas etre organise comme un document global avec des sections uniques melangeant toutes les etapes. Il souhaite une partie par avancement, avec les memes sous-parties repetees a chaque fois : travaux realises, decisions techniques, difficultes, analyse des prompts, points forts, points d'amelioration, blocages, enseignements, observations et notes personnelles.

Correction appliquee : `rapport.md` a ete restructure en `Avancement 1 - TP1` puis `Avancement 2 - TP1.5`. La note personnelle deja ecrite par l'utilisateur a ete conservee dans la partie TP1. La partie TP1.5 garde une section `Notes personnelles` vide pour etre completee plus tard.

## 2026-07-12 - Debut partie 3, WiFi HTTP et page HTML en Flash

L'etape 3 commence apres les tags `tp1-pilotage-local` et `tp1-5-json-serie`. Les documents consultes sont `docs/TP1.75.pdf` pour LittleFS, `docs/TP2.5-wifi.pdf` pour la connexion WiFi en mode station, et `docs/TP3.0-http.pdf` pour le serveur HTTP sur l'ESP.

Decision technique : utiliser `WebServer` avec `WiFi.h` et `LittleFS` plutot que `ESPAsyncWebServer`. Le document du cours presente le serveur asynchrone, mais l'architecture simple est privilegiee ici pour limiter les dependances externes et rester dans une progression maitrisable. Cette decision pourra etre discutee dans le rapport comme un compromis entre fidelite au cours et simplicite.

Le code conserve la regulation locale et le JSON serie des etapes precedentes. La partie HTTP ajoute les routes `/`, `/status`, `/temperature`, `/leds`, `/fan` et `/set`. La page `index.html` et le fichier `esp.css` sont places dans `esp_iot/data/` pour etre uploades dans LittleFS, comme indique par le document TP1.75.

Point de validation a prevoir : l'utilisateur devra remplacer `WIFI_SSID` et `WIFI_PASSWORD` dans le sketch, uploader le dossier `data` en LittleFS, puis uploader le firmware. La page Web devra afficher l'etat de l'ESP et `/status` devra retourner le JSON.

## 2026-07-12 - Mise a jour du rapport pour la partie 3

Le rapport a ete complete avec un nouvel `Avancement 3` dedie a la partie WiFi, HTTP et LittleFS. La structure par avancement est conservee, avec les memes sous-parties que pour TP1 et TP1.5.
