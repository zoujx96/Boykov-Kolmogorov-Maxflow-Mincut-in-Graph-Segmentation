/*!/usr/bin/env c
-*- coding: utf-8 -*-
**************************************
@Time    : 2018/9/27 0:50
@Author  : Jiaxu Zou
@File    : min_cut.c 
**************************************
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*Represent the isolation between vertexes, or invalid data stored here in the array.*/
#define INF 10000

/*Mincut Function to find Mincut*/ 
void mincut(void); 

/*Phase of path growth*/
int growth(void); 

/*Phase of path augmentation*/
void augmentation(void); 

/*Phase of path restoration*/
void adoption(void); 

int i,j,k;
int a,b,c;

/*Represent vertexes*/ 
int v,w;

/*Number of vertexes and edges*/
int numv,nume; 

/*Source vertex and Terminal vertex*/
int s,t;

/*Point to active vertexes, orphan vertexes, respectively*/ 
int ptra,ptro;

/*Bottleneck capacity*/
int cost;

/*Remaining graph*/
int Gr[10000][10000];

/*Active vertexes array, Orphan vertexes array, Tree array, Father vertexes array, Path array*/
int A[10000], O[10000], Tree[10000], P[10000], path[10000];

int main(){
	/*Initialization*/
	ptra = 0;
	ptro = 0;

	/*Type in the number of vertexes and edges*/
	scanf("%d%d", &numv, &nume);

	for(i = 0;i <= numv;i++){
		A[i] = INF;
		O[i] = INF;
		P[i] = INF;
		Tree[i] = INF;
		path[i] = INF;
	}

	for(i = 0;i <= numv;i++)
		for(j = 0;j <= numv;j++){
			Gr[i][j] = INF;
		}
	
	/*Type in the information about the directed graph and the capacity of each edge*/
	for(i = 1;i <= nume;i++){
		scanf("%d%d%d", &a, &b, &c);
		Gr[a][b] = c;
	}

	/*Find the Mincut*/
	mincut();

	/*Output the vertexes divided to Set S(Source) by Mincut*/
	for(i = 1;i <= numv;i++){
		if(Tree[i] == 1)
			printf("%d ",i);
	}

	printf("\n");
	
	/*Output the vertexes divided to Set T(Terminal) by Mincut*/
	for(i = 1;i <= numv;i++){
		if(Tree[i] == 2)
			printf("%d ",i);
	}

	printf("\n");

	return 0;
}

/*Implement Mincut Function*/
void mincut(void){

	/*Find Vertex S(Source) and T(Terminal)*/
	for(i = 1;i <= numv;i++){
		for(j = 1;j <= numv;j++)
			if(Gr[j][i] != INF)
				break;

		if(j == numv + 1){
			A[ptra++] = i;
			Tree[i] = 1;
			s = i;
		}

		for(j = 1;j <= numv;j++)
			if(Gr[i][j] != INF)
				break;

		if(j == numv + 1){
			A[ptra++] = i;
			Tree[i] = 2;
			t = i;
		}
	}

	while(1){
		/*Phase of path growth*/
		a = growth();

		/*Fail to grow the path from S to T, meaning that Mincut has been found*/
		if(a == 0)
			break;

		/*Phase of path augmentation*/
		augmentation();

		/*Phase of path restoration*/
		adoption();
	}
	return;
}

int growth(void){
	while(ptra != 0){

		/*Get an active vertex*/
		v = A[ptra-1];

		if(Tree[v] == 1){
			for(i = 1;i <= numv;i++){
				/*Judge whether the vertex belongs to Set S*/
				if(Gr[v][i] != INF && Gr[v][i] > 0){
					if(Tree[i] == INF){
						Tree[i] = Tree[v];
						P[i] = v;

						/*Mark neighbor vertex as active vertex*/
						A[ptra++] = i;
					}

					/*Find the path*/
					else{
						/*Store the path array*/
						if(Tree[i] != Tree[v]){
							path[v] = i;
							w = v;
							while(w != s){
								path[P[w]] = w;
								w = P[w];
							}
							w = i;
							while(w != t){
								path[w] = P[w];
								w = P[w];
							}
							return path[s];
						}
					}
				}
			}
		}
		
		else{
			for(i = 1;i <= numv;i++){

				/*The vertex belongs to Set T*/
				if(Gr[i][v] != INF && Gr[i][v] > 0){
					if(Tree[i] == INF){
						Tree[i] = Tree[v];
						P[i] = v;
						/*Mark neighbor vertex as active vertex*/
						A[ptra++] = i;
					}

					/*Find the path*/
					else{
						/*Store the path array*/
						if(Tree[i] != Tree[v]){
							path[i] = v;
							w = i;
							while(w != s){
								path[P[w]] = w;
								w = P[w];
							}
							w = v;
							while(w != t){
								path[w] = P[w];
								w = P[w];
							}
							return path[s];
						}
					}
				}
			}
		}

		/*Turn the vertex to inactive vertex*/
		A[--ptra]=INF;
	}

	/*Fail to find the path*/
	return 0;
}

void augmentation(void){
	w = s;
	cost = Gr[w][path[w]];
	while(w != t){
		if(Gr[w][path[w]] < cost){
			cost = Gr[w][path[w]];
		}
		w = path[w];
	}

	/*Find the bottleneck capacity of the path*/
	w = s;
	while(w != t){

		/*Augment the path and update the remaining graph*/
		Gr[w][path[w]] = Gr[w][path[w]] - cost;

		/*Process the saturated edge*/
		if(Gr[w][path[w]] == 0){
			if(Tree[w] == 1 && Tree[path[w]] == 1){
				P[path[w]] = INF;
				O[ptro++] = path[w];
			}
			if(Tree[w] == 2 && Tree[path[w]] == 2){
				P[w] = INF;
				O[ptro++] = w;
			}
		}

		w = path[w];
	}
	return;	
}

void adoption(void){
	/*Process orphan vertex*/
	while(ptro != 0){
		/*Get one orphan vertex*/
		v = O[ptro-1];
		O[--ptro] = INF;
		if(Tree[v] == 1){
		
			/*Find a new father vertex for the orphan vertex*/
			for(i = 1;i <= numv;i++){
				c = 0;
				if(Gr[i][v] != INF && Gr[i][v] > 0){
					if(Tree[v] == Tree[i]){
						w = i;
						while(w != s && w != t){
							if(w == INF)
								break;
							w = P[w];
						}
						if(w == s || w == t){
							P[v] = i;
							c = 1;
						}
					}
				}

				if(c == 1)
					/*Succeed in finding the new father vertex*/
					break;
			}
		}

		else{
			/*Find a new father vertex for the orphan vertex*/
			for(i = 1;i <= numv;i++){
				c = 0;
				if(Gr[v][i] != INF && Gr[v][i] > 0){
					if(Tree[v] == Tree[i]){
						w = i;
						while(w != s && w != t){
							if(w == INF)
								break;
							w = P[w];
						}
						if(w == s || w == t){
							P[v] = i;
							c = 1;
						}
					}
			}

			if(c == 1)
				/*Succeed in finding the new father vertex*/ 
				break;
			}
		}

		if(c != 1){
			/*Fail to find the new father vertex*/
			for(i = 1;i <= numv;i++){
				if(Tree[v] == Tree[i]){
					if(Gr[i][v] != INF && Gr[i][v] > 0)
						A[ptra++]=i;

					if(P[i] == v){
						O[ptro++] = i;
						P[i] = INF;
					}
				}
			}

			Tree[v] = INF;
			i = 0;
			while(A[i] != v){
				i++;
			}

			A[ptra] = INF;
			ptra--;
			A[ptra] = INF;
			/*Remove the vertex from the set of active vertexes and free the orphan vertex*/
			while(i != ptra){
				A[i] = A[i+1];
				i++;
			}
	    }
	}
	return;
}
