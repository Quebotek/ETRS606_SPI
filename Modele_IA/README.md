TP1 : Problème MNIST

Le jeu de données MNIST est un benchmark classique en apprentissage automatique pour la
classification d’images. Il contient des images en niveaux de gris de chiffres manuscrits (0 à 9). Chaque image
est de taille 28×28 pixels, soit 784 pixels au total. 

L’objectif de ce TP est de prédire correctement la classe (le chiffre) associée à chaque image parmi les 10 classes possibles 
(0–9).

<img width="293" height="294" alt="image" src="https://github.com/user-attachments/assets/c1679630-d716-4586-9a5e-af310b81faf5" />


Pour entraîner un réseau de neurones avec la base MNIST, chaque image [28 × 28] est transformée
en un vecteur aplati [1 × 784]. Ainsi, la position spatiale des pixels n’est plus explicitement exploitée dans le
modèle. Nous allons implémenter un modèle de réseau des neurones denses, aussi connu comme multi-layer
perceptron (MPL) ou fully connected. Il sera composé :

-d'une couche d'entrée de dimension 784
-d'une couche de sortie dense de 10 neurones, produisant une distribution de probabilité sur les 10 classes.
-d'un nombre des couches cachées différentes selon le besoin.

Une sortie correspond à la probabilité qu'une image corresponde à une classe donnée. 
L'appretissage sera réalisé par descente de gradient en minimisant l'entropie croisée.

Une descente de gradient est un algorithme d'optimisation qui entraine les machines par réduction des erreurs entre les
résultats prédits et les résultats réels.

L’entropie croisée est une métrique qui peut être utilisée pour refléter la précision des prévisions probabilistes.
L’entropie croisée revêt une importance primordiale pour les systèmes de prévision modernes, car elle joue un rôle
essentiel dans la production de prévisions supérieures, même lorsque d’autres métriques sont utilisées.
Elle est particulièrement importante car elle soutient l’estimation de modèles qui parviennent 
également à capturer les probabilités d’événements rares, lesquels se révèlent souvent être les plus coûteux.

Nous avons donc préparé le code et testé chaque modele d'activation qui sont les suivants :

-Baseline (Softmax)
-ReLu
-tanh
-sigmoid

Nous allons ensuite comparer les résultats et choisir le meilleur compromis entre efficacité et coût.

Dans un second temps nous allons choisir l'architecture la plus adaptée pour notre projet entre :

-SGD
-Adam
-RMSprop
-Adagrad

Dans un dernier temps, nous allons choisir la fonction coût en fonction de nos besoins. Nous avons le choix parmi :

-binary_crossentropy
-categorical_crossentropy
-sparse_categorical_crossentropy

