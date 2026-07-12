# Règles générales
Les règles suivantes sont obligatoires.

- Toujours privilégier les documents présents dans `./docs` avant d'utiliser des connaissances générales.
- Si une information des documents contredit tes connaissances, considère le document comme la source de vérité.
- Ne jamais sauter une étape du projet.
- Toujours proposer un plan avant de modifier plusieurs fichiers.
- Expliquer les choix techniques lorsqu'ils ne sont pas évidents.
- Réutiliser au maximum le code déjà développé.
- Préférer une architecture simple plutôt qu'une architecture complexe.

# Contexte du projet
Ce projet est réalisé dans le cadre d'un rattrapage du Master 1 DS4H (Université Côte d'Azur), pour la matière IoT.
L'objectif du projet n'est pas uniquement de produire une application fonctionnelle, mais également de démontrer une utilisation pertinente de l'intelligence artificielle tout au long du développement.

# Workflow
Pour chaque nouvelle demande :
1. Identifier à quelle étape du projet elle appartient.
2. Vérifier les documents concernés dans `./docs`.
3. Expliquer brièvement le plan.
4. Réaliser uniquement ce qui est demandé.
5. Vérifier que la modification ne casse pas les étapes précédentes.
6. Mettre à jour `notes.md` si la tâche est significative.

Si plusieurs fichiers doivent être modifiés ou si une décision d'architecture est nécessaire, toujours expliquer les impacts avant de générer le code.



# Contexte pédagogique
Le but n'est pas uniquement de produire une application fonctionnelle.

Le projet doit montrer :
- comment une IA peut être utilisée dans un développement logiciel ;
- comment les prompts influencent le résultat ;
- quelles corrections ont été nécessaires ;
- quelles limites de l'IA ont été rencontrées.

Lorsque cela est pertinent, explique les décisions afin qu'elles puissent être réutilisées dans le rapport de rattrapage.

# Documentation
Le dossier `./docs` contient les sources officielles du projet.

Il peut notamment contenir :
- les TP du professeur 
- les cours 
- les photos de l'ESP 
- les photos du matériel 
- les schémas fonctionnels 

Ces documents constituent la référence principale du projet.

# À ne jamais faire

- Ne jamais ignorer les documents du professeur.
- Ne jamais inventer une API.
- Ne jamais supprimer du code sans justification.
- Ne jamais passer au TP suivant sans validation.
- Ne jamais introduire une technologie non demandée.

# Les bases du projet
 1) TP 1 - Pilotage local de l'ESP
 La première étape consiste à vérifier que l'ESP fonctionne correctement en local, sans réseau.
 Le programme doit permettre de piloter directement les composants connectés à l'ESP.
 Les premiers périphériques concernés sont :
 - LED
 - Capteur de température
 - Ventilateur
 - éventuellement d'autres sorties numériques
 L'objectif n'est donc pas uniquement d'allumer une LED, mais de mettre en place une première logique de contrôle.
 Exemple :
 - lecture d'une température
 - comparaison avec un seuil
 - activation du ventilateur
 - extinction lorsque la température redescend 
 Cette partie doit rester totalement locale.
 Aucune communication HTTP, MQTT ou Node-RED n'est encore utilisée.
 Le code doit être suffisamment propre pour que la logique de régulation puisse être réutilisée plus tard.
 Tu peux utiliser le document dans ./docs/TP1.0, notement page 50, partie 11.1
 Il faut aussi prendre en compte la notion de "bruit". Dans ce contexte, il ne s'agit pas de bruit sonore ou de bruit dans une image, mais plutôt de bruit de mesure.
 Une mesure de température peut être instable, la régulation ne doit donc pas se baser uniquement sur une seule valeur instantanée. 
 Avant de comparer la température au seuil, le programme doit appliquer une logique simple de filtrage.
 Exemples possibles :
 - moyenne des 5 dernières mesures 
 - confirmation sur plusieurs mesures consécutives 
 - délai minimal avant changement d'état du ventilateur 
 Par exemple, si le seuil est à 28°C, le ventilateur s'allume seulement si plusieurs mesures confirment une température supérieure à 28°C. Aussi il ne s'éteint pas immédiatement à 27.9°C. Enfin il peut s'éteindre uniquement lorsque la température redescend sous 27°C.
 
 2) TP 1.5 -  Communication série et JSON
 Une fois la régulation fonctionnelle, l'ESP doit commencer à communiquer avec un ordinateur via l'USB. POur ca, il faut utiliser le json
 Le fonctionnement attendu est :
 - récupération des informations des capteurs
 - création d'un objet JSON
 - affichage du JSON sur le port série
 Pour le json il y a un exemple qu'il faut pas hésitez a recuperer comme model a la page 28 du fichier ./docs/TP1.5-json. 
 L'utilisation d'un format JSON propre est importante car il sera réutilisé dans toutes les étapes suivantes.
 Pour la partie node red tu peux utiliser le document ./docs/TP1.5-nr
 
 3) TP1.75 / TP3.0 / TP2.5 - Connexion WiFi et HTTP
 À cette étape, l'ESP quitte la communication série pour devenir un objet connecté au réseau.
 Ici on mentionne : 
 - connexion WiFi
 - HTTP
 - mémoire Flash de l'ESP
 - page HTML
 L'idée est que l'ESP héberge un petit serveur Web.
 Le navigateur doit pouvoir accéder directement à l'ESP.
 Une interface HTML simple doit permettre :
 - d'afficher les valeurs des capteurs
 - de connaître l'état du ventilateur
 - de connaître l'état des LEDs
 - éventuellement d'envoyer quelques commandes
 Le code HTML est stocké directement dans la mémoire Flash de l'ESP.
 Cette étape permet de vérifier que :
- le WiFi fonctionne
- le serveur HTTP fonctionne
- les échanges entre navigateur et ESP fonctionnent correctement
 
 4) TP 4.2 - MQTT
 Une fois les communications HTTP maîtrisées, le projet doit évoluer vers MQTT.
 L'ESP ne communique plus directement avec un navigateur mais publie ses informations sur un broker MQTT.
 Les différents composants du projet pourront alors :
 - publier des mesures
 - recevoir des commandes
 - s'abonner aux différents topics
 Les informations envoyées devront toujours rester au format JSON afin de conserver une structure cohérente.
 Cette partie servira ensuite de base à l'intégration avec Node-RED, MongoDB et Render.
 
# Philosophie general
Je souhaite que le projet soit construit progressivement. Chaque etape doit être terminée et validée et avant de commencer la suivante. 
Par exemple, ne jamais commencer MQTT tant que :  
- la régulation locale fonctionne,
- les échanges JSON sont corrects,
- le serveur HTTP fonctionne.
Chaque étape doit pouvoir être démontrée indépendamment. Pour ca, j'aimerais que le projet soit sur git et qu'on fasse un tag a chaque fois qu'une fonctionnalité est validé afin de pouvoir en parler facilement dans mon rapport
Le code doit rester modulaire afin que les fonctionnalités développées précédemment soient simplement réutilisées dans les étapes suivantes.
Le code principal de l'ESP doit rester dans un dossier unique qui évolue au fur et à mesure du projet. Il ne faut pas créer un nouveau dossier de code pour chaque TP. L'historique des versions validées doit être retrouvé grâce aux tags Git.
Ne pas créer de README ou de fichier de validation séparé pour chaque TP sauf demande explicite. La documentation détaillée doit être préparée dans `rapport.md` au moment de la validation/tag, à partir de `notes.md`.

# Journal de bord (`notes.md`)
Le fichier `./notes.md` doit être considéré comme un journal de bord.
Après chaque échange significatif ou chaque tâche importante, ajoute de nouvelles notes en fin de fichier.
Ce journal n'a pas besoin d'être parfaitement structuré. Il sert uniquement à conserver des observations qui pourront être utilisées plus tard.
Tu peux notamment y noter :
- la qualité et la précision de mes prompts ;
- les difficultés rencontrées ;
- les erreurs ou incompréhensions de ma part ;
- les points où mes demandes étaient particulièrement claires ;
- les décisions techniques importantes ;
- les changements d'orientation du projet ;
- les bonnes pratiques ou mauvaises pratiques observées ;
- les points qui pourraient être intéressants à évoquer dans le rapport final.

N'efface jamais les anciennes notes : ajoute simplement les nouvelles à la fin.
 
# Rapport (`rapport.md`)
À chaque création d'un nouveau tag Git (ou lorsqu'il est explicitement demandé de préparer le rapport correspondant à un tag), génère ou mets à jour le fichier `./rapport.md`.
Ce rapport doit être construit à partir des informations présentes dans `notes.md`.
Le rapport doit être clair, structuré et rédigé de manière professionnelle.
Il doit contenir au minimum les sections suivantes :
- Résumé de la période couverte par le tag.
- Travaux réalisés.
- Décisions techniques importantes.
- Difficultés rencontrées.
- Analyse de la qualité de mes prompts.
- Mes points forts dans l'utilisation de l'IA.
- Mes points d'amélioration.
- Les situations où nous avons perdu du temps ou rencontré des blocages.
- Les enseignements tirés de cette étape.
- Toute autre observation pertinente que tu jugeras utile.

À la fin du rapport, ajoute systématiquement une section : 
'## Notes personnelles
Cette section doit rester vide afin que je puisse compléter le rapport moi-même.'
 
 
 
 
 
 
