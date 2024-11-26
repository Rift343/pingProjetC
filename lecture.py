import csv
import numpy as np
import matplotlib.pyplot as plt
import statistics

def read_and_display_csv(file_path, color):
    with open(file_path, 'r') as file:
        Value = dict()
        reader = csv.reader(file, delimiter=';')
        for row in reader:
            if len(row) == 3:
                try:
                    numFork = int(row[0])
                    temps = float(row[1])
                    tempsT = float(row[2])
                    if numFork in Value:
                        Value[numFork].append(temps)
                    else:
                        Value[numFork] = [temps]
                except ValueError:
                    print(f"Ligne mal formée ignorée: {row}")
            else:
                print(f"Ligne mal formée ignorée: {row}")
    
    avgValue = {key: statistics.mean(Value[key]) for key in Value}
    print(avgValue)
    
    plt.xlabel('Nombre de socket')
    plt.ylabel('temps en seconde')
    plt.scatter(avgValue.keys(), avgValue.values(), c=color, label=file_path)
    plt.plot(list(avgValue.keys()), list(avgValue.values()), color=color)

# Appel de la fonction avec le chemin du fichier CSV
read_and_display_csv('./client/ping_times.txt', "r")  # Red color
read_and_display_csv('./clientThread/ping_times.txt', "b")  # Blue color

plt.legend()
plt.savefig('plot.png')  # Save the plot as a PNG file
print("Plot saved as 'plot.png'. You can open it to view the plot.")
