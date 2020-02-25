from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#plt.style.use('fivethirtyeight')

x_vals = []
y_vals = []

index = count()

def animate(i):
    data = pd.read_csv('lectura4.csv')
    y_vals = data['adc_1']

    plt.cla()
    plt.plot(y_vals)
    plt.tight_layout()


ani = FuncAnimation(plt.gcf(),animate,interval=200)

plt.tight_layout()
plt.show()
