import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple, Dict, Callable, Union
import time
from sklearn.linear_model import LinearRegression

def split(t: np.ndarray)-> Tuple[np.ndarray, int, np.ndarray]:
    """Reparte los elementos de t entre dos arrays con los elementos menores y mayores que t[0] y devuelve una tupla con los elementos menores, el elemento t[0] y los elementos mayores."""
    try:
        pivot = t[0]
        a=[]
        b=[]
        for i in range(1,len(t)):
            if t[0]>t[i]:
                a.append(t[i])
            else:
                b.append(t[i])
    except: 
        print("Errror in fucnt split")
    else:
        return (a,pivot,b)
        


def qsel(t: np.ndarray, k: int)-> Union[int, None]:
    """ Apliqua de manera recursiva el algoritmo QuickSelect usando la función split anterior y devuelve el valor del elemento que ocuparía el índice k en una ordenación de t"""
    try:
        if len(t)==1 and k==0:
            return t[0]
        a,mid,b=split(t)
        
        if (k==len(a)):
            return mid
        if (k<len(a)):
            return qsel(a,k)
        elif(k>len(a)):
            return qsel(b,k-len(a)-1)
        return 
    except:
        print("Error in funct qsel")



def qsel_nr(t: np.ndarray, k: int)-> Union[int, None]:
    """Elimina la recursión de cola de la función anterior"""
    aux=t.copy()
    try:
        while len(aux)>0:
            if (len(aux)==1 and k==0):
                return aux[0]

            a,mid,b=split(aux)

            if (k==len(a)):
                return mid
            elif (k<len(a)):
                aux=a
            elif(k>len(a)):
                aux=b
                k-=(len(a)+1)
        return
    except:
        print("Error in funct qsel_nr")
        
        

def split_pivot(t: np.ndarray, mid: int)-> Tuple[np.ndarray, int, np.ndarray]:
    """modifica la funcion split anterior de manera que usa el valor mid para dividir t"""
    try:
        t_l = [u for u in t if u < mid]
        t_r = [u for u in t if u > mid]
    except:
        print("Error in funct split_pivot")
    else:
        return (t_l, mid, t_r)



def pivot5(t: np.ndarray)-> int:
    """devuelve el “pivote 5”del array t de acuerdo al procedimiento “mediana de medianas de 5 elementos” """
    try:
        if(len(t)>5):
            ngroups = len(t) // 5
            indicemed = 5 // 2 

            sublistas =  [t[i:i+ 5] for i in range(0, len(t), 5)][:ngroups]

            medianas = [sorted(sub)[indicemed] for sub in sublistas]

            if len(medianas) <= 5:
                pivote = sorted(medianas)[len(medianas)//2]
            else:
                pivote = qsel5_nr(medianas, len(medianas)//2)
            return pivote
        else:
            return np.sort(t)[len(t)//2]
    except:
        print("Error in funct pivot5")




def qsel5_nr(t: np.ndarray, k: int)-> int:
    """devuelve el elemento en el índice k de una ordenación de t utilizando la funciones pivot5, split_pivot"""
    if k >= len(t) or k < 0:
        return
    
    try:
        tt = t.copy()
        while len(tt) > 5:
            mid = pivot5(np.array(tt))

            t_l, mid, t_r = split_pivot(tt, mid)
            m = len(t_l)

            if k == m:
                return int(mid)
            elif k < m:
                tt = t_l
            else:
                tt = t_r
                k = k-m-1

        if len(tt) <= 5:
            return int(np.sort(tt)[k])
    except:
        print("Error in funct qsel5_nr")
        



def qsort_5(t: np.ndarray)-> np.ndarray:
    """utiliza las funciones anteriores split_pivot, pivot_5 para devolver una ordenación de la tabla t"""
    if len(t)==0:
        return np.array([])
    aux= t.copy()
    
    try:
        t_l, mid, t_r = split_pivot(aux, pivot5(aux))
        if(len(t_l)>1):
            t_l=qsort_5(t_l)
        if(len(t_r)>1):
            t_r=qsort_5(t_r)

        t_l=np.append(t_l, np.array([mid]))
    except:
        print("Error in funct qsort_5")
    else:
        return np.append(t_l,t_r)
        
        
        
        
def edit_distance(str_1: str, str_2: str)-> int:
    """devuelve la distancia de edición entre las cadenas str_1, str_2"""
    if len(str_1) == 0 or len(str_2) == 0:
        return None
    c = 0
    try:
        if len(str_1) < len(str_2):
            for i in range(0, len(str_1)):
                if str_1[i] != str_2[i]:
                    c += 1
            c += len(str_2) - len(str_1)
        else:
            for i in range(0, len(str_2)):
                if str_1[i] != str_2[i]:
                    c += 1
            c += len(str_1) - len(str_2)
    except:
        print("Error in funct edit distance")
    else:
        return c 
        



def max_subsequence_length(str_1: str, str_2: str)->int:
    """devuelve la longitud de una subsecuencia común a las cadenas str_1, str_2 aunque no necesariamente consecutiva"""
    e = np.zeros((len(str_1)+1, len(str_2)+1), dtype=int)
    try:
        for i in range(1, len(str_1)+1):
            for j in range(i, len(str_2)+1):
                if (str_1[i-1] == str_2[j-1]):
                    e[i,j] = 1 + e[i-1, j-1]
                else :
                    e[i, j] = max(e[i-1, j], e[i, j-1])
    except:
        print("Error in funct max subsequence length")
    else:
        return e[-1,-1]




def max_common_subsequence(str_1: str, str_2: str)-> str:
    """devuelve una subcadena com ́un a las cadenas str_1, str_2 aunque no necesariamente consecutiva."""	
    aux = []
    k=0
    try:
        for i in range(0, len(str_1)):
            for j in range(k, i+1):
                if str_1[i] == str_2[j]:
                    aux.append(str_1[i])
                    k = j
                    break
    except:
        print("Error in funct max common subsequence")
    else:
        return aux
        
        
        
def min_mult_matrix(l_dims: List[int]) -> int:
    """devuelve el número mínimo de productos para multiplicar n matrices cuyas dimensiones están contenidas en la lista l_dims"""
    size = len(l_dims)-1
    m_1= np.inf * np.ones((size, size))
    np.fill_diagonal(m_1, 0)
    try:
        for i in range(0, size-1):
            m_1[i, i+1] = l_dims[i] * l_dims[i+1]* l_dims[i+2]
    
        for j in range(2, size):
            for l in range(size - j):
                for m in range(l, j+l):
                    m_1[l, j+l]= min(m_1[l, j+l], l_dims[l] * l_dims[m+1] * l_dims[j+l+1] + m_1[l,m] + m_1[m+1,j+l])
    except:
        print("Error in funct min mult matrix")
    else:
        return m_1        



