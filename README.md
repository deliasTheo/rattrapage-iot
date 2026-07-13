# Rattrapage IoT - Methode de travail avec IA

Ce depot contient le projet realise pour le rattrapage IoT du Master 1 DS4H. Le but n'etait pas seulement de produire du code fonctionnel, mais aussi de montrer comment l'intelligence artificielle a ete utilisee pendant le developpement.

## Methode generale

Le projet a ete construit progressivement, etape par etape. Chaque partie devait etre terminee avant de passer a la suivante :

1. pilotage local de l'ESP ;
2. communication serie et JSON ;
3. WiFi, HTTP et page HTML en memoire Flash ;
4. MQTT.

L'idee etait d'eviter de tout melanger. Par exemple, MQTT n'a ete ajoute qu'apres la regulation locale, le JSON et le serveur HTTP.

Le code principal de l'ESP se trouve dans un seul dossier :

```text
esp_iot/
```

Ce dossier evolue au fur et a mesure du projet. Les anciennes versions ne sont pas conservees dans des dossiers separes : elles sont retrouvees avec les tags Git.

## Role de `docs/`

Le dossier `docs/` contient les documents fournis pour le projet :

- les fichiers de TP ;
- les cours ;
- les photos du materiel ;
- les schemas.

Ces documents ont servi de reference principale. Quand une information du cours etait disponible, elle etait prioritaire sur les connaissances generales de l'IA. Cela a notamment servi pour les brochages imposes, le modele JSON, LittleFS, HTTP et MQTT.

## Role de `AGENTS.md`

Le fichier `AGENTS.md` sert a donner les consignes de travail a Codex. Il decrit :

- les regles generales du projet ;
- l'ordre des etapes ;
- les documents a consulter ;
- les limites a ne pas depasser ;
- la maniere de tenir `notes.md` et `rapport.md` ;
- la logique des tags Git.

Je me suis aide de ChatGPT pour reflechir au nom de ce fichier, a son role, a sa structure et a ce qu'il fallait ecrire dedans. Ensuite, ce fichier a servi de cadre de travail pour Codex pendant le developpement.

## Role de `notes.md`

Le fichier `notes.md` est un journal de bord. Il contient des observations prises au fur et a mesure :

- decisions techniques ;
- erreurs ou corrections ;
- limites de l'IA ;
- changements d'organisation ;
- points utiles pour le rapport.

Ce fichier n'est pas redige comme un rapport final. Il sert plutot de trace brute du deroulement du projet.

## Role de `rapport.md`

Le fichier `rapport.md` est la version structuree et propre du suivi du projet. Il est construit a partir de `notes.md`.

Il est organise par avancement. Chaque partie reprend les memes sous-sections :

- resume ;
- travaux realises ;
- decisions techniques ;
- difficultes rencontrees ;
- analyse de la qualite des prompts ;
- points forts ;
- points d'amelioration ;
- blocages ;
- enseignements ;
- observations ;
- notes personnelles.

Le rapport sert donc a expliquer le projet de maniere professionnelle, alors que `notes.md` conserve plutot le detail du cheminement.

## Role de Codex

Codex a ete utilise comme assistant de developpement dans le depot local. Il permet de travailler directement sur les fichiers du projet :

- lire les documents ;
- analyser le code existant ;
- proposer un plan avant les modifications importantes ;
- modifier les fichiers ;
- mettre a jour le journal de bord ;
- preparer le rapport ;
- creer les commits ;
- creer et pousser les tags Git.

Dans ce projet, Codex a surtout servi a transformer les consignes et les documents de TP en code progressif pour l'ESP32, tout en gardant une trace des choix et des limites rencontrees.

L'utilisation de Codex a aussi montre une limite importante : l'IA peut parfois proposer des fichiers ou des outils utiles en theorie, mais non demandes. Par exemple, des fichiers d'exemple et un validateur JSON avaient ete proposes puis retires pour respecter plus strictement le perimetre demande.

## Role de ChatGPT

ChatGPT a ete utilise en dehors du depot pour reflechir a certains aspects de methode :

- comment organiser les consignes ;
- comment nommer et structurer `AGENTS.md` ;
- comment formuler certaines attentes avant de les appliquer dans le projet.

La difference principale est que ChatGPT a servi a reflechir et cadrer, tandis que Codex a servi a appliquer ces choix directement dans le depot.

## Tags Git

Chaque etape validee du projet est reperee par un tag Git. Cela permet au correcteur de retrouver l'etat exact du code a un moment donne.

Tags disponibles :

```text
tp1-pilotage-local
tp1-5-json-serie
tp3-wifi-http-littlefs
tp4-mqtt
```

Pour consulter une etape precise :

```bash
git checkout tp1-pilotage-local
git checkout tp1-5-json-serie
git checkout tp3-wifi-http-littlefs
git checkout tp4-mqtt
```

Le dernier tag, `tp4-mqtt`, correspond a la version finale du projet.

## Organisation principale du depot

```text
AGENTS.md      consignes donnees a Codex
docs/          documents de cours, TP, photos et schemas
esp_iot/       code principal ESP32 et fichiers HTML/CSS LittleFS
notes.md       journal de bord brut
rapport.md     rapport structure par avancement
README.md      explication de la methode et des outils utilises
```

