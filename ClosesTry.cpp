#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Timer.cpp"

int count = 0;
struct Point{
	int x, y;
};

Point* p;
Point a, b;
float dmin = 10000;
void readFile();								// Doc va khoi tao diem ban dau
void printPoint(Point*);								// in danh sach cac diem
float dist(Point, Point);							// khoang cach giua hai diem o, q
void mergeSort(Point*, int, int, int);
void merge(Point*, int, int ,int, int); 		// sap xep tron theo x voi flag = 0, theo y voi flag = 1
float bruteForce(Point*, int);				// Thuat toan vet can
void setPoint(Point*, Point*);
float min(float, float);
float stripClosest(Point*, int, float);

void readFile(){
	FILE* f = fopen("point.txt","rt");
	if(f==NULL) {
		printf("Khong mo duoc file");
		exit(0);
	}
	fscanf(f, "%d", &count);
	p = (Point*) malloc(count * sizeof(Point));
	int x, y;
	for(int i = 0; i < count; i++){
		fscanf(f,"%d",&x);
		fscanf(f,"%d",&y);
		p[i].x = x;
		p[i].y = y;
	}
	fclose(f);
}

void setPoint(Point* p, Point* q){
	for(int i = 0; i < count; i++){
		q[i] = p[i];
	}
}

void printPoint(Point* p){
	printf("Danh sach cac diem:\n");
	for(int i = 0; i< count; i++){
		printf("P%d(%d,%d) ",i+1, p[i].x, p[i].y);
		if (i%5 == 4){
			printf("\n");
		}		
	}
}

float dist(Point p, Point q){
	return sqrt(pow(p.x-q.x,2) + pow(p.y-q.y,2));
}

float min(float x, float y){
	if (x > y) return  y;
	else return x;
}

float bruteForce(Point *p, int n){
	float min = 10000;
	for(int i = 0; i < n; i++){
		for(int j = i+1; j < n; j++){
			float d = dist(p[i], p[j]);
			if (d < min){
				min = d;
				if (min < dmin){				
					a.x = p[i].x; b.x = p[j].x;
					a.y = p[i].y; b.y = p[j].y;
				}
			}
		}
	}
	dmin = min;
	return min;
}

void merge(Point* p, int L, int M, int R, int flag){
	int i =L; int j = M+1;
	Point* tmp = (Point*) malloc((R + 1) * sizeof(Point));
	for(int k = L; k <= R; k++){
		if(i > M){
			tmp[k] = p[j];
			j++;	
		} 
		else if(j>R){
			tmp[k] = p[i];
			i++;
		}
		else {
			int s1;
			int s2;
			if (flag == 0){
				s1 = p[i].x;
				s2 = p[j].x;
			}
			else{
				s1 = p[i].y;
				s2 = p[j].y;
			}
			if (s1 < s2){
				tmp[k] = p[i];
				i++;
			}
			else {
				tmp[k] = p[j];
				j++;
			}
		}
	}
	for(int k = L; k <=R; k++){
		p[k] = tmp[k];
	}
	free(tmp);
}

void mergeSort(Point* p, int L, int R, int flag){
	if(L < R){
		int M = (L+R)/2;
		mergeSort(p, L, M, flag);
		mergeSort(p, M+1, R, flag);
		merge(p,L, M, R, flag);
	}
}

float stripClosest(Point* pLR, int k, float d){
	float min = d;
	printf("StripClosest: %f\n", min);
	mergeSort(pLR,0,k-1, 1);
	for(int i = 0; i < k; i++) printf("%d ", pLR[i].y);
	printf("\n");
	for (int i = 0; i < k; ++i){
		for (int j = i+1; j < k && (pLR[j].y - pLR[i].y) < min; ++j){
			if (dist(pLR[i],pLR[j]) < min){
				min = dist(pLR[i],pLR[j]);
				if (min < dmin){
					dmin = min;
					a.x = p[i].x; b.x = p[j].x;
					a.y = p[i].y; b.y = p[j].y;
				}
			}
			printf("StripClosest: %f\n", min);
		}
	}
	return min;
}

void print(Point* p, int n){
	printf("Danh sach cac diem:\n");
	for(int i = 0; i< n; i++){
		printf("P%d(%d,%d) ",i+1, p[i].x, p[i].y);
		if (i%5 == 4){
			printf("\n");
		}		
	}
}

float ClosestPair(Point* pX, int n){
	if(n <= 3) return bruteForce(pX, n);
	
	//Chia mang thanh hai phan bang nhau
	int mid = n/2;
	Point midPoint = pX[mid];

	float dL = ClosestPair(pX, mid);
	printf("ClosestPairL %f\n", dL);
	float dR = ClosestPair(pX + mid,n-mid);
	printf("ClosestPairR %f\n", dR);
	
	float d = min(dL, dR);
	printf("ClosestPair %f\n", d);

	Point* pLR = (Point*) malloc (n * sizeof(Point));
	int k = 0;
	for (int i = 0; i < n; ++i){
		if (abs(pX[i].x - midPoint.x) <= d){
			pLR[k] = pX[i];
			k++;
		}
	}
	if (dmin > min(stripClosest(pLR, k, d), d)){
		dmin = min(stripClosest(pLR, k, d), d);
	}
	return min(stripClosest(pLR, k, d), d);;
}

int main(int argc, char const *argv[]){
	readFile();
	Point* pX = (Point*) malloc(count * sizeof(Point));
	mergeSort(p,0,count-1, 1);
	printPoint(p);
	mergeSort(p,0,count-1, 0);
	setPoint(p, pX);
	printf("Co %d diem tren mat phang.\n", count);
	printPoint(p);
//	Timer ti;
//	bruteForce(p, count);
//	double y = ti.getElapsedTime();
//	printf("Thoi gian chay: %f\n",y);
//	printf("Hai diem gan nhau nhat la:\nA(%d, %d), B(%d, %d)\n",a.x,a.y,b.x,b.y);
	float min = ClosestPair(pX, count);
	printf("Khoang cach gan nhat la: %f\n", min);
	printf("Hai diem gan nhau nhat la:\nA(%d, %d), B(%d, %d)\n",a.x,a.y,b.x,b.y);
	
	return 0;
}