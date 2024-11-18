import csv
import numpy as np
import matplotlib.pyplot as plt
import statistics;

def read_and_display_csv(file_path):
    with open(file_path, 'r') as file:
        Value = dict();
        reader = csv.reader(file, delimiter=';')
        for row in reader:
            if len(row) == 3:
                try:
                    numFork = int(row[0])
                    temps = float(row[1])
                    tempsT = float(row[2])
                    if(numFork in Value):
                        Value[numFork].append(temps)
                    else:
                        Value[numFork] = []
                        Value[numFork].append(temps)
                        
                except ValueError:
                    print(f"Ligne mal formée ignorée: {row}")
                    
            else:
                print(f"Ligne mal formée ignorée: {row}")
        avgValue = dict()
        for key in Value:
            avgValue[key] = statistics.mean(Value[key])
        print(avgValue)
        plt.xlabel('Nombre de socket')
        plt.ylabel('temps en seconde')
        for key in avgValue:
            plt.plot(key,avgValue[key],'ro')
            print("ok")
        plt.show()
# Appel de la fonction avec le chemin du fichier CSV
read_and_display_csv('ping_times.txt')