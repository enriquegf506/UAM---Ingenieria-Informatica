{
 "cells": [
  {
   "cell_type": "code",
   "execution_count": 1,
   "id": "92b9c4ac",
   "metadata": {},
   "outputs": [],
   "source": [
    "import numpy as np\n",
    "import matplotlib.pyplot as plt"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 2,
   "id": "0a7baf7c",
   "metadata": {},
   "outputs": [],
   "source": [
    "def matrix_multiplication(m_1: np.ndarray, m_2: np.ndarray)-> np.ndarray:\n",
    "    \"\"\"Recibe dos matrices Numpy de dimensiones compatibles \n",
    "    y devuelve otra matriz Numpy con su producto.\"\"\"\n",
    "    rows1, cols1 = m_1.shape\n",
    "    rows2, cols2 = m_2.shape\n",
    "    m_3 = np.zeros(shape=(rows1, cols2), dtype=int)\n",
    "    if cols1 == rows2:\n",
    "        for i in range(0,rows1):\n",
    "            for j in range(0, cols2):\n",
    "                for n in range(0, rows2):\n",
    "                    m_3[i][j] += m_1[i][n] * m_2[n][j]\n",
    "    return m_3"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 3,
   "id": "0923745a",
   "metadata": {},
   "outputs": [],
   "source": [
    "def bb(t: list, f: int, l: int, key: int)-> int:\n",
    "    \"\"\" Recibe una lista, sus  ́ındices first, last primero y  ́ultimo, y una clave key , \n",
    "    y encuentra la posicion de key entre first y last.\n",
    "    Si no la encuentra, devuelve None .\"\"\"\n",
    "    medio: int\n",
    "    while f <= l:\n",
    "        medio= (f+l)//2\n",
    "        if t[medio]== key:\n",
    "            return medio\n",
    "        elif t[medio] > key:\n",
    "            l = medio-1\n",
    "        else:\n",
    "            f = medio+1\n",
    "\n",
    "    return None"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 4,
   "id": "6b04aa6e",
   "metadata": {},
   "outputs": [],
   "source": [
    "def rec_bb(t: list, f: int, l: int, key: int)-> int:\n",
    "    \"\"\" Recibe una lista, sus  ́ındices first, last primero y  ́ultimo, y una clave key , \n",
    "    y aplique una version recursiva de la busqueda binaria para encontrar \n",
    "    la posicion de key entre first y last .\n",
    "    Si no la encuentra, devolver ́a None .\"\"\"\n",
    "    medio = (l - f) // 2 + f\n",
    "    if f <= l:\n",
    "        if t[medio] <key:\n",
    "            f=medio + 1\n",
    "            return rec_bb(t, f, l, key)\n",
    "        elif t[medio] > key:\n",
    "            l=medio - 1\n",
    "            return rec_bb(t, f, l , key)\n",
    "        elif t[medio] == key:\n",
    "            return medio\n",
    "        else:\n",
    "            return None\n",
    "    else:\n",
    "        return None"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 5,
   "id": "e70e1da1",
   "metadata": {},
   "outputs": [],
   "source": [
    "def min_heapify(h: np.ndarray, i: int):\n",
    "    \"\"\"Recibe un array h de Numpy y aplica la operacion de heapify \n",
    "    al elemento situado en la posicion i\"\"\"\n",
    "    while 2*i+1 < len(h):\n",
    "        n_i = i\n",
    "        if h[i] > h[2*i+1]:\n",
    "            n_i = 2*i+1\n",
    "        if 2*i+2 < len(h) and h[i] > h[2*i+2] and h[2*i+2] < h[n_i]:\n",
    "            n_i = 2*i+2\n",
    "        if n_i > i:\n",
    "            h[i], h[n_i] = h[n_i], h[i]\n",
    "            i = n_i\n",
    "        else:\n",
    "            return"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 6,
   "id": "c4629a60",
   "metadata": {},
   "outputs": [],
   "source": [
    "def insert_min_heap(h: np.ndarray, k: int)-> np.ndarray:\n",
    "    \"\"\"Inserta el entero k en el min heap contenido en h y devuelve el nuevo min heap.\"\"\"\n",
    "    h += [k]\n",
    "    j = len(h) - 1\n",
    "    while j >= 1 and h[(j-1) // 2] > h[j]:\n",
    "        h[(j-1) // 2], h[j] = h[j], h[(j-1) // 2]\n",
    "        j = (j-1) // 2\n",
    "    min_heapify(h, k)\n",
    "    return h"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 7,
   "id": "d130ac72",
   "metadata": {},
   "outputs": [],
   "source": [
    "def create_min_heap(h: np.ndarray):\n",
    "    \"\"\"Crea un min heap sobre el array de Numpy pasado como argumento\"\"\"\n",
    "    m = len(h) // 2\n",
    "    for j in range(m, -1, -1):\n",
    "        i=j\n",
    "        while 2*i+1 < len(h):\n",
    "            n_i = i\n",
    "            if h[i] > h[2*i+1]:\n",
    "                n_i = 2*i+1\n",
    "            if 2*i+2 < len(h) and h[i] > h[2*i+2] and h[2*i+2] < h[n_i]:\n",
    "                n_i = 2*i+2\n",
    "            if n_i > i:\n",
    "                h[i], h[n_i] = h[n_i], h[i]\n",
    "                i = n_i\n",
    "            else:\n",
    "                break\n",
    "    return\n"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 8,
   "id": "250126f6",
   "metadata": {},
   "outputs": [],
   "source": [
    "def pq_ini():\n",
    "    \"\"\"inicializa una cola de prioridad vacıa.\"\"\"\n",
    "    np.ndarray: c\n",
    "    c=[]\n",
    "    return c"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 9,
   "id": "3fddc912",
   "metadata": {},
   "outputs": [],
   "source": [
    "def pq_insert(h: np.ndarray, k: int)-> np.ndarray:\n",
    "    \"\"\"inserta el elemento k en la cola de prioridad h y devuelva la nueva cola.\"\"\"\n",
    "    h= insert_min_heap(h,k)\n",
    "    return h"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 10,
   "id": "b7ef9522",
   "metadata": {},
   "outputs": [],
   "source": [
    "def pq_remove (h: np.ndarray) -> tuple[int, np.ndarray]:\n",
    "    \"\"\"Elimina el elemento con el menor valor de prioridad de h y \n",
    "    devuelve dicho elemento y la nueva cola.\"\"\"\n",
    "    h[0], h[len(h)-1] = h[len(h)-1], h[0]\n",
    "    h[len(h)-1] = None\n",
    "    min_heapify(h, 0)\n",
    "    return h[0], h"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 11,
   "id": "f5e39ad7",
   "metadata": {},
   "outputs": [],
   "source": [
    "def select_min_heap(h: np.ndarray, k: int)-> int:\n",
    "    \"\"\"Indica que numero del array estaría en la posición k si este estuviera ordenado.\n",
    "    Esto lo hace mediante la creación de max heaps de k elementos.\"\"\"\n",
    "    \n",
    "    m=[]\n",
    "    if k> len(h):\n",
    "        return None\n",
    "    \n",
    "    #Pasamos la array a su negativo para que el min heap sea un max heap\n",
    "    for i in range (0, len(h)):\n",
    "        h[i] = h[i]*(-1) \n",
    "    \n",
    "    #Creamos el array\n",
    "    for i in range (0, k):\n",
    "        m.append(h[i])\n",
    "    \n",
    "    create_min_heap(m)\n",
    "    i = k-1\n",
    "    #metemos el array los k elementos más pequños\n",
    "    while i < len(h)-1:\n",
    "        if h[i+1] > m[0]:\n",
    "            m[0] = h[i+1]\n",
    "            min_heapify(m, 0)\n",
    "        i += 1\n",
    "        \n",
    "    #Pasamos la array a positivo para que el min heap sea un max heap\n",
    "    for i in range (0, len(h)):\n",
    "        h[i] = h[i]*(-1) \n",
    "    \n",
    "    return m[0] * (-1) "
   ]
  }
 ],
 "metadata": {
  "kernelspec": {
   "display_name": "Python 3 (ipykernel)",
   "language": "python",
   "name": "python3"
  },
  "language_info": {
   "codemirror_mode": {
    "name": "ipython",
    "version": 3
   },
   "file_extension": ".py",
   "mimetype": "text/x-python",
   "name": "python",
   "nbconvert_exporter": "python",
   "pygments_lexer": "ipython3",
   "version": "3.9.12"
  }
 },
 "nbformat": 4,
 "nbformat_minor": 5
}
