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
