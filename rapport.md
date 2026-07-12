# Rapport - TP1 : Pilotage local de l'ESP

## Resume de la periode couverte par le tag

Cette periode couvre le demarrage du projet IoT et la realisation de la premiere etape : le TP1 de pilotage local de l'ESP32. L'objectif etait de produire une regulation locale de temperature sans utiliser WiFi, HTTP, MQTT, Node-RED ou JSON.

Le code correspondant se trouve dans `esp_iot/esp_iot.ino`. Ce dossier est destine a evoluer au fil des prochaines etapes. L'etat TP1 sera retrouve grace au tag Git associe.

## Travaux realises

- Consultation des consignes projet dans `AGENTS.md`.
- Consultation du document officiel `docs/TP1.0.pdf`, notamment la page 50, section 11.1.
- Identification des broches imposees par le TP :
  - capteur de temperature sur `GPIO 23` ;
  - LED verte de climatisation sur `GPIO 19` ;
  - LED rouge de chauffage sur `GPIO 21` ;
  - ventilateur sur `GPIO 27`.
- Creation du sketch Arduino principal dans `esp_iot/esp_iot.ino`.
- Mise en place d'une regulation locale avec lecture de temperature, seuil bas, seuil haut, chauffage, climatisation et ventilateur.
- Ajout d'un filtrage simple par moyenne des 5 dernieres mesures.
- Ajout d'une hysteresis et d'un delai minimal entre deux changements d'etat pour limiter l'effet du bruit de mesure.
- Ajout d'une option de PWM pour le ventilateur, desactivee par defaut tant que le montage moteur n'est pas confirme.
- Correction de l'organisation du projet pour garder un dossier de code unique evolutif.

## Decisions techniques importantes

La principale decision a ete de rester strictement dans le perimetre du TP1. Aucune communication reseau, aucun format JSON et aucune integration Node-RED ou MQTT n'ont ete ajoutes. Cela respecte la progression demandee et evite de melanger les etapes.

La regulation ne s'appuie pas sur une mesure instantanee. Une moyenne glissante de 5 mesures est utilisee pour reduire l'impact du bruit de mesure. Une hysteresis de `1.0 C` evite les basculements rapides autour des seuils. Un delai minimal de `5 s` limite aussi les changements d'etat trop frequents.

Le ventilateur est commande en tout-ou-rien par defaut. Une commande PWM existe dans le code, mais reste desactivee car le montage materiel n'a pas encore ete confirme. Ce choix evite de supposer un circuit moteur que le projet ne garantit pas encore.

L'organisation du projet a ete corrigee apres retour utilisateur. Le code n'est pas range dans un dossier par TP. Il reste dans un dossier unique, `esp_iot`, qui sera modifie progressivement. Les tags Git servent a retrouver les versions validees.

## Difficultes rencontrees

La premiere difficulte a ete materielle : l'ESP n'etait pas branche et le capteur DHT n'etait pas clairement identifie au depart. Une partie de l'echange a donc porte sur la reconnaissance du capteur et sur le branchement possible.

Une autre difficulte concernait l'organisation du projet. Une premiere proposition avait cree une documentation separee pour TP1. Cette approche ne correspondait pas a la logique souhaitee. Elle a ete corrigee pour privilegier un dossier de code evolutif et une documentation centralisee dans le rapport.

Enfin, la compilation automatique n'a pas pu etre faite depuis le terminal car `arduino-cli` n'etait pas installe. La validation finale repose donc sur la validation utilisateur et, idealement, une verification dans Arduino IDE.

## Analyse de la qualite des prompts

Les prompts etaient globalement clairs sur la progression attendue : commencer par TP1, ne pas sauter d'etape, et rester dans une logique de projet incrementale.

Le retour sur l'organisation du code a ete particulierement utile. Il a permis de corriger une mauvaise interpretation : l'objectif n'etait pas de creer un dossier par TP, mais de maintenir un code unique dont les etats sont retrouves par tags Git.

Les demandes sur le branchement etaient formulees de maniere concrete. Elles ont montre que l'accompagnement devait etre plus visuel et plus progressif pour la partie materielle.

## Mes points forts dans l'utilisation de l'IA

L'utilisateur a donne un contexte projet precis, avec des regles claires dans `AGENTS.md`. Cela a permis de cadrer le developpement et d'eviter de passer trop vite aux etapes suivantes.

L'utilisateur a aussi corrige rapidement une organisation qui ne lui convenait pas. Cette intervention a ameliore la structure du projet et a clarifie la methode de travail pour les prochaines etapes.

## Mes points d'amelioration

Il faudra continuer a valider explicitement l'organisation avant de creer plusieurs fichiers. Une proposition courte de structure doit etre faite avant toute modification importante.

Il faudra aussi distinguer plus clairement trois niveaux :
- le code ;
- le journal de bord `notes.md` ;
- le rapport final `rapport.md`.

La documentation intermediaire ne doit pas etre dispersee dans des fichiers supplementaires sauf demande explicite.

## Situations ou nous avons perdu du temps ou rencontre des blocages

Du temps a aussi ete perdu sur la premiere organisation des fichiers. La creation d'un dossier TP1 dedie et de fichiers de validation separes ne correspondait pas a la philosophie attendue. Cette erreur a ete corrigee avant la validation.

## Enseignements tires de cette etape

Cette etape montre l'interet d'une progression stricte : commencer par une regulation locale simple avant d'ajouter les communications.

Elle montre aussi que les tags Git sont utiles pedagogiquement. Ils permettent de retrouver un etat valide du code sans dupliquer les dossiers ou figer artificiellement chaque TP dans une arborescence differente.

Enfin, elle confirme qu'un projet IoT doit traiter le bruit de mesure des le depart. Une regulation basee sur une seule mesure instantanee serait trop instable.

## Autres observations pertinentes

Le code est volontairement simple pour rester compatible avec le niveau TP1. Il pourra etre restructure progressivement si les prochaines etapes l'exigent, mais sans introduire de complexite inutile trop tot.

## Notes personnelles
Pour l'instant, l'IA se debrouille plutot bien. J'ai passé pas mal de temps aussi avec chatgpt en dehors du projet pour essayer de voir ce qu'il me conseillé comme nom de fichier, pour le 'AGENTS.md' par exemple, ou ce qu'il fallais mettre dedans. A voir si pour le reste du projet ca se passe aussi bien. 
