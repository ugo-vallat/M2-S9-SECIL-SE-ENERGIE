import random

nb_values = 10_000_000
# Générer une liste de 10 000 valeurs aléatoires entre 0 et 1000
values = [random.randint(0, 1000) for _ in range(nb_values)]

# Trier la liste dans l'ordre croissant
values_sorted = sorted(values)

