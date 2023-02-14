#include <stdio.h>
#include "point.h"

int main(){
	int i;
	Point *p[4] = {NULL, NULL, NULL, NULL};

	p[0] = point_new(0, 0, BARRIER);
	if (!p[0]){
		fprintf(stdout, "Error reservando memoria p[0]");
		return 0;
	}

	p[1] = point_new(0, 1, BARRIER);
	if (!p[1]) {
        fprintf(stdout, "Error reservando memoria p[1]");
		point_free(p[0]);
		return 0;
	}

	if(point_print(stdout, p[0]) < 0){
		fprintf(stdout, "Error imprimiendo p[0]");
		point_free(p[0]);
		point_free(p[1]);
		return 0;
	}
	if(point_print(stdout, p[1]) < 0){
		fprintf(stdout, "Error imprimiendo p[1]");
		point_free(p[0]);
		point_free(p[1]);
		return 0;
	}
	fprintf(stdout, "\n");


	fprintf(stdout, "Equal points p[0] and p[1]? ");
	if (point_equal(p[0], p[1]) == FALSE){
		fprintf(stdout, "No\n");
	}else{
		fprintf(stdout, "Yes\n");
	}


	fprintf(stdout, "Creating p[2]: ");
	p[2] = point_hardcpy(p[0]);
	if(p[2] == NULL){
		fprintf(stdout, "Error creando p[2]");
		point_free(p[0]);
		point_free(p[1]);
		return 0;
	}
	if(point_print(stdout, p[2]) < 0){
		fprintf(stdout, "Error imprimiendo p[2]");
		point_free(p[0]);
		point_free(p[1]);
		point_free(p[2]);
		return 0;
	}


	fprintf(stdout, "\nEqual points p[0] and p[2]? ");
        if (point_equal(p[0], p[2]) == FALSE){
            fprintf(stdout, "No\n");
        }else{
            fprintf(stdout, "Yes\n");
			fprintf(stdout, "Modifying p[2]: ");
			if(point_setSymbol(p[2], SPACE) == ERROR){
				fprintf(stdout, "Error asignando simbolo a p[2]");
				point_free(p[0]);
				point_free(p[1]);
				point_free(p[2]);
			}
			if(point_print(stdout, p[2]) < 0){
				fprintf(stdout, "Error imprimiendo p[2]");
				point_free(p[0]);
				point_free(p[1]);
				point_free(p[2]);
				return 0;
			}
        }


	fprintf(stdout, "\nEqual points p[0] and p[2]? ");
        if (point_equal(p[0], p[2]) == FALSE){
                fprintf(stdout, "No\n");
        }else{
                fprintf(stdout, "Yes\n");
        }


	fprintf(stdout, "Assign p[3] = p[0]\n");
	
	p[3] = p[0];
	if(p[3] == NULL){
		fprintf(stdout, "Error creando p[3]");
		point_free(p[0]);
		point_free(p[1]);
		point_free(p[2]);
		return 0;
	}
	

	fprintf(stdout, "Modifying p[3]: ");
	if(point_setSymbol(p[3], OUTPUT) == ERROR){
		fprintf(stdout, "Error asignando simbolo a p[3]");
		point_free(p[0]);
		point_free(p[1]);
		point_free(p[2]);
		point_free(p[3]);
	}
	if(point_print(stdout, p[3]) < 0){
		fprintf(stdout, "Error imprimiendo p[3]");
		point_free(p[0]);
		point_free(p[1]);
		point_free(p[2]);
		point_free(p[3]);
		return 0;
	}

	fprintf(stdout, "\n");

	for(i=0;i<4;i++){
		if(point_print(stdout, p[i])<0){
			fprintf(stdout, "Error imprimiendo p[%d]", i);
			point_free(p[0]);
			point_free(p[1]);
			point_free(p[2]);
			point_free(p[3]);
			return 0;
		}
	}

	fprintf(stdout, "\n");

	for(i=0;i<3;i++){
		point_free(p[i]);
		p[i] = NULL;
	}

	return 0;

}
