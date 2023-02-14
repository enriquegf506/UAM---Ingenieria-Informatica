import numpy as np
import itertools

def init_cd(n: int)-> np.ndarray:
    """Devuelve un array con valores −1 en las posiciones {0, 1, ..., n-1}."""
    try:
        if n < 0:
            return None
        return -1 * np.ones(n).astype(int)
    except:
        print("Error en init_cd")
    
def union(rep_1: int, rep_2: int, p_cd: np.ndarray)-> int:
    """Devuelve el representante del conjunto obtenido como la union por rangos de los representados por los  ́ındices
            rep_1, rep_2 en el CD almacenado en el array p_cd."""
    try:
        if rep_1 > len(p_cd) or rep_2 > len(p_cd):
            return None
        if p_cd[rep_2] < p_cd[rep_1]:
            p_cd[rep_1] = rep_2; return rep_2
        elif p_cd[rep_2] > p_cd[rep_1]:
            p_cd[rep_2] = rep_1; return rep_1
        else:
            p_cd[rep_2] = rep_1; p_cd[rep_1] -= 1; return rep_1
    except:
        print("Error en union")

def find(ind: int, p_cd: np.ndarray)-> int:
    """Devuelve el representante del ́ındice ind en el CD almacenado en p_cd realizando compresión de caminos."""
    try:
        if ind > len(p_cd) or ind < 0:
            return None
        z = ind
        while p_cd[z] >= 0:
            z = p_cd[z]
        while p_cd[ind] >= 0:
            y = p_cd[ind]
            p_cd[ind] = z
            ind = y
        return z
    except:
        print("Error en find")
    
def cd_2_dict(p_cd: np.ndarray)-> dict:
    """Recibe un CD en el array p_cd y devuelve un diccionario cuyas claves sean los representantes de los subconjuntos
        del CD y donde el valor de la clave u del dict sea una lista con los miembros del subconjunto """
    try:
        d = {}
        for i in range(0,len(p_cd)):
            if(p_cd[i]<0):
                l=[]
                for j in range (0,len(p_cd)):
                    m=find(j, p_cd)
                    if(m==i):
                        l.append(j)
                d[i]=l
        return d
    except:
        print("Error en cd_2_dict")
    
def ccs(n: int, l: list)-> dict:
    """Devuelve las componentes conexas de un tal grafo mediante un diccionario."""
    try:
        cd=init_cd(n)
        for i in l: 
            p1=find(i[0],cd)
            p2=find(i[1],cd)
            j=union(p1,p2,cd)
        return cd_2_dict(cd)
    except:
        print("Error en ccs")
    
def dist_matrix(n_nodes: int, w_max=10)-> np.ndarray:
    """Genera la matriz de distancias de un grafo con n_nodes nodos, valores enteros con un maximo w_max"""
    try:
        m_1=[]
        m_1 = np.random.randint(0, w_max, (n_nodes, n_nodes))
        m_1 = (m_1 + m_1.T) // 2
        m_1 = m_1 - np.diag( np.diag(m_1))
        return m_1
    except:
        print("Error en dist_matrix")
    
def greedy_tsp(dist_m: np.ndarray, node_ini=0)-> list:
    """Recibe una matriz de distancias y un nodo inicial y devuelve un circuito codiciosos como una lista con valores entre
        0 y el n ́umero de nodos menos 1."""
    try:
        num_cities = dist_m.shape[0]
        circuit = [node_ini]
        while len(circuit) < num_cities:
            current_city = circuit[-1]
            options = list( np.argsort(dist_m[ current_city ]))
            for node in options:
                if node not in circuit:
                    circuit.append(node)
                    break
        return circuit + [node_ini]
    except:
        print("Error en greedy_tspgreedy_tsp")
    
def len_circuit(circuit: list, dist_m: np.ndarray)-> int:
    """Recibe un circuito y una matriz de distancias y devuelve la longitud de dicho circuito."""
    try:
        i=0
        dist=0
        while i < (len(circuit) - 1):
            dist += dist_m[circuit[i]][circuit[i+1]]
            i += 1
        return dist
    except:
        print("Error en len_circuit")
    
def repeated_greedy_tsp(dist_m: np.ndarray)-> list:
    """Llama a greedy_tsp a partir de todos los nodos del grafo y devuelve el circuito con la menor longitud."""
    try:
        best_circuit=greedy_tsp(dist_m,0)
        current_n=len_circuit(best_circuit,dist_m)
        for i in range(1,len(dist_m)):
            new_circuit=greedy_tsp(dist_m,i)
            new_n=len_circuit(new_circuit,dist_m)
            if(new_n<current_n):
                best_circuit=new_circuit
                current_n=new_n

        return best_circuit
    except:
        print("Error en repeated_greedy_tsp")
    
def exhaustive_tsp(dist_m: np.ndarray)-> list:
    """Examina todos los posibles circuitos y devuelve aquel con la distancia más corta."""
    try:
        best_len=0
        nodes=dist_m.shape[0]
        for permutation in itertools.permutations(range(nodes)):
            perm=list(permutation)
            perm.append(perm[0])
            new_len=len_circuit(perm,dist_m)
            if(best_len>new_len or best_len==0):
                best_len=new_len
                best_perm=perm
        return best_perm
    except:
        print("Error en exhaustive_tsp")
    
    
    
