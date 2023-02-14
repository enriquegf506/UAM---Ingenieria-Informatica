import numpy as np
import matplotlib.pyplot as plt
from typing import List, Callable

def matrix_multiplication(m_1: np.ndarray, m_2: np.ndarray)-> np.ndarray:
    """Recibe dos matrices Numpy de dimensiones compatibles 
    y devuelve otra matriz Numpy con su producto."""
    rows1, cols1 = m_1.shape
    """Guarda filas y columnas de m_1"""
    rows2, cols2 = m_2.shape
    """Guarda filas y columnas de m_2"""
    try:
        m_3 = np.zeros(shape=(rows1, cols2), dtype=int)
        if cols1 == rows2:
            for i in range(0,rows1):
                for j in range(0, cols2):
                    for n in range(0, rows2):
                        m_3[i][j] += m_1[i][n] * m_2[n][j]
    except:
        print("Error al multipicar matrices")
    else:
        return m_3

def bb(t: list, f: int, l: int, key: int)-> int:
    """ Recibe una lista, sus  ́ındices first, last primero y  ́ultimo, y una clave key , 
    y encuentra la posicion de key entre first y last.
    Si no la encuentra, devuelve None ."""
    medio: int
    try:
        while f <= l:
            medio= (f+l)//2
            if t[medio]== key:
                return medio
            elif t[medio] > key:
                l = medio-1
            else:
                f = medio+1
    except:
        print("Error en la busqueda binaria")
    else:
        return None

def rec_bb(t: list, f: int, l: int, key: int)-> int:
    """ Recibe una lista, sus  ́ındices first, last primero y  ́ultimo, y una clave key , 
    y aplique una version recursiva de la busqueda binaria para encontrar 
    la posicion de key entre first y last .
    Si no la encuentra, devolver ́a None ."""
    medio = (l - f) // 2 + f
    if f <= l:
        if t[medio] <key:
            f=medio + 1
            return rec_bb(t, f, l, key)
        elif t[medio] > key:
            l=medio - 1
            return rec_bb(t, f, l , key)
        elif t[medio] == key:
            return medio
        else:
            return None
    else:
        return None
    
def min_heapify(h: np.ndarray, i: int):
    """Recibe un array h de Numpy y aplica la operacion de heapify 
    al elemento situado en la posicion i"""
    
    if(i<0 or i> len(h)):
        return
        
    while 2*i+1 < len(h):
        n_i = i
        if h[i] > h[2*i+1]:
            n_i = 2*i+1
        if 2*i+2 < len(h) and h[i] > h[2*i+2] and h[2*i+2] < h[n_i]:
            n_i = 2*i+2
        if n_i > i:
            h[i], h[n_i] = h[n_i], h[i]
            i = n_i
        else:
            return
        
def insert_min_heap(h: np.ndarray, k: int)-> np.ndarray:
    """Inserta el entero k en el min heap contenido en h y devuelve el nuevo min heap."""
    h = np.append(h,[k])
    j = len(h) - 1
    try:
        while j >= 1 and h[(j-1) // 2] > h[j]:
            h[(j-1) // 2], h[j] = h[j], h[(j-1) // 2]
            j = (j-1) // 2
    except:
        print("Error insertando")
    else:
        return h

def create_min_heap(h: np.ndarray):
    """Crea un min heap sobre el array de Numpy pasado como argumento"""
    m = len(h) // 2
    try:
        for j in range(m, -1, -1):
            i=j
            while 2*i+1 < len(h):
                n_i = i
                if h[i] > h[2*i+1]:
                    n_i = 2*i+1
                if 2*i+2 < len(h) and h[i] > h[2*i+2] and h[2*i+2] < h[n_i]:
                    n_i = 2*i+2
                if n_i > i:
                    h[i], h[n_i] = h[n_i], h[i]
                    i = n_i
                else:
                    break
    except:
        print("Error creando min heap")
    
    else:
        return


def pq_ini():
    """inicializa una cola de prioridad vacıa."""
    try:
        np.ndarray: c
        c=[]
        """Inicializa el array"""
    except:
        print("Error iniciando pq")
    else:
        return c

def pq_insert(h: np.ndarray, k: int)-> np.ndarray:
    """inserta el elemento k en la cola de prioridad h y devuelva la nueva cola."""
    try:
        h= insert_min_heap(h,k)
    except:
        print("Error insertando en pq")
    else:
        return h

def pq_remove (h: np.ndarray) -> tuple[int, np.ndarray]:
    """Elimina el elemento con el menor valor de prioridad de h y 
    devuelve dicho elemento y la nueva cola."""
    try:
        remove=h[0]
        h[0]= h[len(h)-1]
        min_heapify(h[1:], 0)
    except:
        print("Error eliminando de pq")
    else:
        return remove, h[1:]
        


def select_min_heap(h: np.ndarray, k: int)-> int:
    """Indica que numero del array estaría en la posición k si este estuviera ordenado.
    Esto lo hace mediante la creación de max heaps de k elementos."""
    
    m=[]
    """Nuevo array de k elementos"""
    if k> len(h):
        return None
    
    #Pasamos la array a su negativo para que el min heap sea un max heap
    for i in range (0, len(h)):
        h[i] = h[i]*(-1) 
    
    #Creamos el array
    for i in range (0, k):
        m.append(h[i])
    
    create_min_heap(m)
    i = k-1
    #metemos el array los k elementos más pequños
    while i < len(h)-1:
        if h[i+1] > m[0]:
            m[0] = h[i+1]
            min_heapify(m, 0)
        i += 1
        
    #Pasamos la array a positivo para que el min heap sea un max heap
    for i in range (0, len(h)):
        h[i] = h[i]*(-1) 
    
    return m[0] * (-1) 


