# define _CRT_SECURE_NO_WARNINGS

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <time.h>
# include <windows.h>
# include "opencv2/opencv.hpp"

# define initial_population 100 //100
# define initial_population2 0 //30
# define initial_food_population 200 //200
# define initial_starve 40 //40
# define initial_starve2 40 //40
# define food_limit 200 //200

# define grid 600 // 격자 크기
# define food_create_probability 0.80 // 1 interval당 음식이 생길 확률(2자리 소수)
# define createfood 1 // interval 당 생기는 음식의 양

# define pi 3.141592

# define NIN 36 //number of input neuron
# define NON 3 // number of output neuron
# define NHN 40 // number of hidden neuron
# define NHL 2 // number of hidden layer

# define range 10 // 시야 1칸의 길이
# define theta 15 // 시야 1각의 크기
# define range2 13
# define theta2 12

# define max_speed 10.0 // 속력의 최대값
# define max_angular_speed 15.0 // 각속력의 최대값
# define eat_distance 0.1 // 포식 범위

# define interval 0.5 // 속력 조절값
# define foodplus 10
# define foodplus2 10
# define mutation_rate 0.1

void input_to_first_hidden_layer(double input[NIN], double hidden[NHL][NHN], double weight_IH[NIN + 1][NHN]);
void hidden_to_hidden_layer(double hidden[NHL][NHN], double weight_HH[NHL - 1][NHN + 1][NHN]);
void hidden_to_output_layer(double hidden[NHL][NHN], double output[NON], double weight_HO[NHN + 1][NON]);
void one_cycle(double weight_IH[NIN + 1][NHN], double weight_HH[NHL - 1][NHN + 1][NHN], double weight_HO[NHN + 1][NON], double input[NIN], double hidden[NHL][NHN], double output[NON]);
double neuron_function(double value);

void perception(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population);

double cal_angle(int a_x, int a_y, int b_x, int b_y);
double cal_distance(int a_x, int a_y, int b_x, int b_y);

void movement(struct object creature[], int population);
void movement2(struct object2 creature2[], int population2);
void print_grid(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population, CvArr* img);
void initialization(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population);
void eat_and_reproduce(struct object creature[], struct stuff food[], int * population, int * food_population, struct object ** dummy_creature, struct stuff ** dummy_food);
void eat_and_reproduce2(struct object creature[], struct object2 creature2[], int * population, int * population2, struct object2 ** dummy_creature2, struct object ** dummy_creature);
void death(struct object creature[], int* population, struct object ** dummy_creature);
void death2(struct object2 creature2[], int* population2, struct object2 ** dummy_creature2);
void create_food(struct stuff food[], int *food_population, struct stuff ** dummy_food);

double pesudo_sigmoid(double x);
int rounding(double num);


struct stuff {
	int x;
	int y;
	int on;
};

struct object {
	int x;
	int y;
	double angle; //from 0 to 360, x축 양의 방향으로부터의 각도

	double weight_IH[NIN + 1][NHN]; // input neuron 에서 hidden neuron 으로의 weighting factor
	double weight_HH[NHL - 1][NHN + 1][NHN]; // hidden neuron 에서 hidden neuron 으로의 weighting factor
	double weight_HO[NHN + 1][NON]; // hidden neuron 에서 output neuron 으로의 weighting factor

	double input[NIN]; // input neuron 의 값
	double hidden[NHL][NHN];
	double output[NON]; // output neuron 의 값
	// output[0]은 움직이는 속력
	// output[1], [2]은 움직이는 각속력

	double starve;

	int on;

};

struct object2 {
	int x;
	int y;
	double angle; //from 0 to 360, x축 양의 방향으로부터의 각도

	double weight_IH[NIN + 1][NHN]; // input neuron 에서 hidden neuron 으로의 weighting factor
	double weight_HH[NHL - 1][NHN + 1][NHN]; // hidden neuron 에서 hidden neuron 으로의 weighting factor
	double weight_HO[NHN + 1][NON]; // hidden neuron 에서 output neuron 으로의 weighting factor

	double input[NIN]; // input neuron 의 값
	double hidden[NHL][NHN];
	double output[NON]; // output neuron 의 값
						// output[0]은 움직이는 속력
						// output[1]은 움직이는 각속력

	double starve;

	int on;

};

int main() {
	IplImage *display;
	display = cvCreateImage(cvSize(grid, grid), IPL_DEPTH_8U, 3);

	int generation = 0;

	int population;
	int population2;
	int food_population;
	population = initial_population;
	population2 = initial_population2;
	food_population = initial_food_population;

	srand(time(NULL));
	
	int i, j, k, l;


	struct object * creature;
	creature = (struct object *) malloc(sizeof(struct object)*population);
	struct object ** dummy_creature;
	dummy_creature = &creature;

	struct object2 * creature2;
	creature2 = (struct object2 *) malloc(sizeof(struct object2)*population2);
	struct object2 ** dummy_creature2;
	dummy_creature2 = &creature2;

	struct stuff * food;
	food = (struct stuff *) malloc(sizeof(struct stuff)*food_population);
	struct stuff ** dummy_food = &food;

	initialization(creature, creature2, food, population, population2, food_population);

	printf("1.new start\n2.load\n");
//	printf("1.new start\n2.load object1\n3.load object 2\n4.load both\n");
	int start;
	scanf("%d", &start);

	int file = 1;
	int file2 = 1;
	FILE *fp;
	FILE *fp2;
	fp = fopen("weight.bin", "rb");
	fp2 = fopen("weight2.bin", "rb");
	if (start == 1) {
	}
	else if (start == 2||start==4) {
		fread(&creature[0], sizeof(struct object), 1, fp);
		for (i = 1; i < population; i++) {

			for (j = 0; j < NIN + 1; j++) {
				for (k = 0; k < NHN; k++) creature[i].weight_IH[j][k] = creature[0].weight_IH[j][k] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
			}

			for (j = 0; j < NHL - 1; j++) {
				for (k = 0; k < NHN + 1; k++) {
					for (l = 0; l < NHN; l++) creature[i].weight_HH[j][k][l] = creature[0].weight_HH[j][k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}
			}

			for (j = 0; j < NHN + 1; j++) {
				for (k = 0; k < NON; k++) creature[i].weight_HO[j][k] = creature[0].weight_HO[j][k] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
			}
		}
		fclose(fp);
	}
/*
	else if (start == 3||start==4) {
		fread(&creature2[0], sizeof(struct object2), 1, fp2);
		for (i = 1; i < population2; i++) {

			for (j = 0; j < NIN + 1; j++) {
				for (k = 0; k < NHN; k++) creature2[i].weight_IH[j][k] = creature2[0].weight_IH[j][k] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
			}

			for (j = 0; j < NHL - 1; j++) {
				for (k = 0; k < NHN + 1; k++) {
					for (l = 0; l < NHN; l++) creature2[i].weight_HH[j][k][l] = creature2[0].weight_HH[j][k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}
			}

			for (j = 0; j < NHN + 1; j++) {
				for (k = 0; k < NON; k++) creature2[i].weight_HO[j][k] = creature2[0].weight_HO[j][k] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
			}
		}
		fclose(fp2);
	}
	*/
		while (1) {
			perception(creature, creature2, food, population, population2, food_population);

			for (i = 0; i < population; i++) {
				one_cycle(creature[i].weight_IH, creature[i].weight_HH, creature[i].weight_HO, creature[i].input, creature[i].hidden, creature[i].output);
			}
			for (i = 0; i < population2; i++) {
				one_cycle(creature2[i].weight_IH, creature2[i].weight_HH, creature2[i].weight_HO, creature2[i].input, creature2[i].hidden, creature2[i].output);
			}

			movement(creature, population);
			movement2(creature2, population2);

			eat_and_reproduce(creature, food, &population, &food_population, dummy_creature, dummy_food); creature = *dummy_creature; food = *dummy_food;
			eat_and_reproduce2(creature, creature2, &population, &population2, dummy_creature2, dummy_creature); creature2 = *dummy_creature2; creature = *dummy_creature;
			
			death(creature, &population, dummy_creature); creature = *dummy_creature;
			death2(creature2, &population2, dummy_creature2); creature2 = *dummy_creature2;
			create_food(food, &food_population, dummy_food); food = *dummy_food;
			print_grid(creature, creature2, food, population, population2, food_population, display);

			/*데이터 저장 및 종료*/
				if ((file == 1 && population <= 5 && population > 0)||(generation%1000==0 && population > 0)) {
					fp = fopen("weight.bin", "wb");
					fwrite(&creature[0], sizeof(struct object), 1, fp);
					file = 0;
					fclose(fp);
				}
				if ((file2 == 1 && population2 <= 5 && population2 > 0) || (generation % 1000 == 0 && population2 > 0)) {
					fp2 = fopen("weight2.bin", "wb");
					fwrite(&creature2[0], sizeof(struct object2), 1, fp2);
					file2 = 0;
					fclose(fp2);
				}
			if (population == 0&& population2 == 0) break;
			/*데이터 저장 및 종료*/

			generation = generation + 1;
		}
		printf("%d", generation);

}

void input_to_first_hidden_layer(double input[NIN], double hidden[NHL][NHN], double weight_IH[NIN + 1][NHN]) {
	double sum;
	int i, j;
	for (i = 0; i < NHN; i++) {
		sum = weight_IH[0][i];
		for (j = 0; j < NIN; j++) {
			sum = sum + input[j] * weight_IH[j + 1][i];
		}
		hidden[0][i] = neuron_function(sum);
	}
}

void hidden_to_hidden_layer(double hidden[NHL][NHN], double weight_HH[NHL - 1][NHN + 1][NHN]) {
	int i, j, k;
	double sum;
	for (i = 0; i < NHL - 1; i++) { //현재 layer의 번째수
		for (j = 0; j < NHN; j++) { //다음 layer 의 j번째 neuron
			sum = weight_HH[i][0][j];
			for (k = 0; k < NHN; k++) { //현재 layer 의 k번째 neuron
				sum = sum + weight_HH[i][k + 1][j] * hidden[i][k];
			}
			hidden[i + 1][j] = neuron_function(sum);
		}
	}
}

void hidden_to_output_layer(double hidden[NHL][NHN], double output[NON], double weight_HO[NHN + 1][NON]) {
	int i, j;
	double sum;
	for (i = 0; i < NON; i++) {
		sum = weight_HO[0][i];
		for (j = 0; j < NHN; j++) {
			sum = sum + weight_HO[j + 1][i] * hidden[NHL - 1][j];
		}
		output[i] = sum;
	}
}

void one_cycle(double weight_IH[NIN + 1][NHN], double weight_HH[NHL - 1][NHN + 1][NHN], double weight_HO[NHN + 1][NON], double input[NIN], double hidden[NHL][NHN], double output[NON]) {
	input_to_first_hidden_layer(input, hidden, weight_IH);
	hidden_to_hidden_layer(hidden, weight_HH);
	hidden_to_output_layer(hidden, output, weight_HO);
}

double neuron_function(double value) {
	double out;
	if (value > 0) return value;
	else return 0;
}

void perception(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population) {
	int i, j;
	double distance;
	double delx;
	double dely;
	double angle;

	for (i = 0; i < population; i++) {
		for (j = 0; j < NIN; j++) creature[i].input[j] = 0; //input을 0으로 초기화
	}
	for (i = 0; i < population2; i++) {
		for (j = 0; j < NIN; j++) creature2[i].input[j] = 0; //input을 0으로 초기화
	}

	/* input number

	0   1   2   3			12	13	14	15			24	25	26	27
	4   5   6   7			16	17	18	19			28	29	30	31
	8   9  10  11			20	21	22	23			32	33	34	35
	      ^						  ^							^
	      |						  |							|
	    object(to food)			object(to obj2)			object(to obj1)

	*/

	for (i = 0; i < population; i++) {
		for (j = 0; j < food_population; j++) {
			double angle_between_them = cal_angle(creature[i].x, creature[i].y, food[j].x, food[j].y) - creature[i].angle;
			double distance_between_them = cal_distance(creature[i].x, creature[i].y, food[j].x, food[j].y);

			if (distance_between_them <= range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[8] = creature[i].input[8] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[9] = creature[i].input[9] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[10] = creature[i].input[10] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[11] = creature[i].input[11] + 1;
			}
			else if (distance_between_them <= 2 * range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[4] = creature[i].input[4] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[5] = creature[i].input[5] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[6] = creature[i].input[6] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[7] = creature[i].input[7] + 1;
			}
			else if (distance_between_them <= 3 * range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[0] = creature[i].input[0] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[1] = creature[i].input[1] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[2] = creature[i].input[2] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[3] = creature[i].input[3] + 1;
			}
		}
		for (j = 0; j < population2; j++) {
			double angle_between_them = cal_angle(creature[i].x, creature[i].y, creature2[j].x, creature2[j].y) - creature[i].angle;
			double distance_between_them = cal_distance(creature[i].x, creature[i].y, creature2[j].x, creature2[j].y);

			if (distance_between_them <= range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[20] = creature[i].input[20] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[21] = creature[i].input[21] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[22] = creature[i].input[22] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[23] = creature[i].input[23] + 1;
			}
			else if (distance_between_them <= 2 * range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[16] = creature[i].input[16] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[17] = creature[i].input[17] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[18] = creature[i].input[18] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[19] = creature[i].input[19] + 1;
			}
			else if (distance_between_them <= 3 * range) {
				if (angle_between_them <= 2 * theta && angle_between_them > theta) creature[i].input[12] = creature[i].input[12] + 1;
				else if (angle_between_them <= theta && angle_between_them > 0) creature[i].input[13] = creature[i].input[13] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta) creature[i].input[14] = creature[i].input[14] + 1;
				else if (angle_between_them <= -theta && angle_between_them > -2 * theta) creature[i].input[15] = creature[i].input[15] + 1;
			}
		}
		for (j = 0; j < population; j++) {
			if (i != j) {
				double angle_between_ob1to2 = cal_angle(creature[i].x, creature[i].y, creature[j].x, creature[j].y) - creature[i].angle;
				double distance_between_ob1to2 = cal_distance(creature[i].x, creature[i].y, creature[j].x, creature[j].y);

				if (distance_between_ob1to2 <= range) {
					if (angle_between_ob1to2 <= 2 * theta && angle_between_ob1to2 > theta) creature[i].input[32] = creature[i].input[32] + 1;
					else if (angle_between_ob1to2 <= theta && angle_between_ob1to2 > 0) creature[i].input[33] = creature[i].input[33] + 1;
					else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta) creature[i].input[34] = creature[i].input[34] + 1;
					else if (angle_between_ob1to2 <= -theta && angle_between_ob1to2 > -2 * theta) creature[i].input[35] = creature[i].input[35] + 1;
				}
				else if (distance_between_ob1to2 <= 2 * range) {
					if (angle_between_ob1to2 <= 2 * theta && angle_between_ob1to2 > theta) creature[i].input[28] = creature[i].input[28] + 1;
					else if (angle_between_ob1to2 <= theta && angle_between_ob1to2 > 0) creature[i].input[29] = creature[i].input[29] + 1;
					else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta) creature[i].input[30] = creature[i].input[30] + 1;
					else if (angle_between_ob1to2 <= -theta && angle_between_ob1to2 > -2 * theta) creature[i].input[31] = creature[i].input[31] + 1;
				}
				else if (distance_between_ob1to2 <= 3 * range) {
					if (angle_between_ob1to2 <= 2 * theta && angle_between_ob1to2 > theta) creature[i].input[24] = creature[i].input[24] + 1;
					else if (angle_between_ob1to2 <= theta && angle_between_ob1to2 > 0) creature[i].input[25] = creature[i].input[25] + 1;
					else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta) creature[i].input[26] = creature[i].input[26] + 1;
					else if (angle_between_ob1to2 <= -theta && angle_between_ob1to2 > -2 * theta) creature[i].input[27] = creature[i].input[28] + 1;
				}
			}
		}
	}
	for (i = 0; i < population2; i++) {
		for (j = 0; j < food_population; j++) {
			double angle_between_them = cal_angle(creature2[i].x, creature2[i].y, food[j].x, food[j].y) - creature2[i].angle;
			double distance_between_them = cal_distance(creature2[i].x, creature2[i].y, food[j].x, food[j].y);

			if (distance_between_them <= range2) {
				if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[8] = creature2[i].input[8] + 1;
				else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[9] = creature2[i].input[9] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[10] = creature2[i].input[10] + 1;
				else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[11] = creature2[i].input[11] + 1;
			}
			else if (distance_between_them <= 2 * range2) {
				if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[4] = creature2[i].input[4] + 1;
				else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[5] = creature2[i].input[5] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[6] = creature2[i].input[6] + 1;
				else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[7] = creature2[i].input[7] + 1;
			}
			else if (distance_between_them <= 3 * range2) {
				if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[0] = creature2[i].input[0] + 1;
				else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[1] = creature2[i].input[1] + 1;
				else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[2] = creature2[i].input[2] + 1;
				else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[3] = creature2[i].input[3] + 1;
			}
		}
		for (j = 0; j < population2; j++) {
			if (i != j) {
				double angle_between_them = cal_angle(creature2[i].x, creature2[i].y, creature2[j].x, creature2[j].y) - creature2[i].angle;
				double distance_between_them = cal_distance(creature2[i].x, creature2[i].y, creature2[j].x, creature2[j].y);

				if (distance_between_them <= range2) {
					if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[20] = creature2[i].input[20] + 1;
					else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[21] = creature2[i].input[21] + 1;
					else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[22] = creature2[i].input[22] + 1;
					else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[23] = creature2[i].input[23] + 1;
				}
				else if (distance_between_them <= 2 * range2) {
					if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[16] = creature2[i].input[16] + 1;
					else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[17] = creature2[i].input[17] + 1;
					else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[18] = creature2[i].input[18] + 1;
					else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[19] = creature2[i].input[19] + 1;
				}
				else if (distance_between_them <= 3 * range2) {
					if (angle_between_them <= 2 * theta2 && angle_between_them > theta2) creature2[i].input[12] = creature2[i].input[12] + 1;
					else if (angle_between_them <= theta2 && angle_between_them > 0) creature2[i].input[13] = creature2[i].input[13] + 1;
					else if (angle_between_them <= 0 && angle_between_them > -theta2) creature2[i].input[14] = creature2[i].input[14] + 1;
					else if (angle_between_them <= -theta2 && angle_between_them > -2 * theta2) creature2[i].input[15] = creature2[i].input[15] + 1;
				}
			}
		}
		for (j = 0; j < population; j++) {
			double angle_between_ob1to2 = cal_angle(creature2[i].x, creature2[i].y, creature[j].x, creature[j].y) - creature2[i].angle;
			double distance_between_ob1to2 = cal_distance(creature2[i].x, creature2[i].y, creature[j].x, creature[j].y);

			if (distance_between_ob1to2 <= range2) {
				if (angle_between_ob1to2 <= 2 * theta2 && angle_between_ob1to2 > theta2) creature2[i].input[32] = creature2[i].input[32] + 1;
				else if (angle_between_ob1to2 <= theta2 && angle_between_ob1to2 > 0) creature2[i].input[33] = creature2[i].input[33] + 1;
				else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta2) creature2[i].input[34] = creature2[i].input[34] + 1;
				else if (angle_between_ob1to2 <= -theta2 && angle_between_ob1to2 > -2 * theta2) creature2[i].input[35] = creature2[i].input[35] + 1;
			}
			else if (distance_between_ob1to2 <= 2 * range2) {
				if (angle_between_ob1to2 <= 2 * theta2 && angle_between_ob1to2 > theta2) creature2[i].input[28] = creature2[i].input[28] + 1;
				else if (angle_between_ob1to2 <= theta2 && angle_between_ob1to2 > 0) creature2[i].input[29] = creature2[i].input[29] + 1;
				else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta2) creature2[i].input[30] = creature2[i].input[30] + 1;
				else if (angle_between_ob1to2 <= -theta2 && angle_between_ob1to2 > -2 * theta2) creature2[i].input[31] = creature2[i].input[31] + 1;
			}
			else if (distance_between_ob1to2 <= 3 * range2) {
				if (angle_between_ob1to2 <= 2 * theta2 && angle_between_ob1to2 > theta2) creature2[i].input[24] = creature2[i].input[24] + 1;
				else if (angle_between_ob1to2 <= theta2 && angle_between_ob1to2 > 0) creature2[i].input[25] = creature2[i].input[25] + 1;
				else if (angle_between_ob1to2 <= 0 && angle_between_ob1to2 > -theta2) creature2[i].input[26] = creature2[i].input[26] + 1;
				else if (angle_between_ob1to2 <= -theta2 && angle_between_ob1to2 > -2 * theta2) creature2[i].input[27] = creature2[i].input[28] + 1;
			}
		}
	}
}

double cal_angle(int a_x, int a_y, int b_x, int b_y) {
	//calculate angle from a to b
	int vector_x = b_x - a_x;
	int vector_y = b_y - a_y;
	double angle_rad;
	double magnitude = sqrt(vector_x*vector_x + vector_y*vector_y);
	if (vector_x == 0 && vector_y == 0) return 0;
	else if (vector_x == 0 && vector_y > 0) return 90;
	else if (vector_x == 0 && vector_y < 0) return 270;
	else if (vector_y == 0 && vector_x > 0) return 0;
	else if (vector_y == 0 && vector_x < 0) return 180;
	else if (vector_x > 0 && vector_y > 0) {
		angle_rad = atan(vector_y/vector_x);
		return angle_rad * 180 / pi;
	}
	else if (vector_x < 0 && vector_y > 0) {
		angle_rad = atan(-vector_x / vector_y) + (pi / 2);
		return angle_rad * 180 / pi;
	}
	else if (vector_x < 0 && vector_y < 0) {
		angle_rad = atan(vector_y / vector_x) + pi;
		return angle_rad * 180 / pi;
	}
	else if (vector_x > 0 && vector_y < 0) {
		angle_rad = atan(-vector_x / vector_y) + 1.5*pi;
		return angle_rad * 180 / pi;
	}
}

double cal_distance(int a_x, int a_y, int b_x, int b_y) {
	int vector_x = b_x - a_x;
	int vector_y = b_y - a_y;
	return sqrt(vector_x*vector_x + vector_y*vector_y);
}

void movement(struct object creature[], int population) {
	int i;
	for (i = 0; i < population; i++) {
		creature[i].angle = creature[i].angle + (double)interval*pesudo_sigmoid(creature[i].output[1])*max_angular_speed;
		while (creature[i].angle >= 360) creature[i].angle = creature[i].angle - 360;
		while (creature[i].angle < 0) creature[i].angle = creature[i].angle + 360;
		creature[i].x = creature[i].x + rounding(interval*pesudo_sigmoid(neuron_function(creature[i].output[0]))*max_speed * cos(creature[i].angle*pi / 180));
		creature[i].y = creature[i].y + rounding(interval*pesudo_sigmoid(neuron_function(creature[i].output[0]))*max_speed * sin(creature[i].angle*pi / 180));
//		printf("%f %d %d\n", creature[i].angle, (int)(interval*pesudo_sigmoid(creature[i].output[0])*max_speed * cos(creature[i].angle*pi / 180)), (int)(interval*pesudo_sigmoid(creature[i].output[0])*max_speed * sin(creature[i].angle*pi / 180)));
		creature[i].starve = creature[i].starve - interval/10;
	}
}

void movement2(struct object2 creature2[], int population2) {
	int i;
	for (i = 0; i < population2; i++) {
		creature2[i].angle = creature2[i].angle + (double)interval*pesudo_sigmoid(creature2[i].output[1])*max_angular_speed;
		while (creature2[i].angle >= 360) creature2[i].angle = creature2[i].angle - 360;
		while (creature2[i].angle < 0) creature2[i].angle = creature2[i].angle + 360;
		creature2[i].x = creature2[i].x + rounding(interval*pesudo_sigmoid(neuron_function(creature2[i].output[0]))*max_speed * cos(creature2[i].angle*pi / 180));
		creature2[i].y = creature2[i].y + rounding(interval*pesudo_sigmoid(neuron_function(creature2[i].output[0]))*max_speed * sin(creature2[i].angle*pi / 180));
		creature2[i].starve = creature2[i].starve - interval / 10;
	}
}

void print_grid(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population,CvArr* img) {
	int i, j;
	cvSet(img, cvScalarAll(255));
	
	for (i = 0; i < population; i++) {
		cvEllipse(img, cvPoint(creature[i].x, creature[i].y), cvSize(1, 1), 0, 0, 360, CV_RGB(0, 0, 255));
		cvEllipse(img, cvPoint(creature[i].x, creature[i].y), cvSize(range, range), creature[i].angle, -2*theta, 2*theta, CV_RGB(0, 0, 255));
		cvEllipse(img, cvPoint(creature[i].x, creature[i].y), cvSize(2* range, 2* range), creature[i].angle, -2 * theta, 2 * theta, CV_RGB(0, 0, 255));
		cvEllipse(img, cvPoint(creature[i].x, creature[i].y), cvSize(3* range, 3* range), creature[i].angle, -2 * theta, 2 * theta, CV_RGB(0, 0, 255));
		cvLine(img, cvPoint(creature[i].x, creature[i].y), cvPoint(creature[i].x + (int)3 * range*cos(creature[i].angle*pi/180), creature[i].y + 3 * range*sin(creature[i].angle*pi / 180)), CV_RGB(0, 0, 255));
		cvLine(img, cvPoint(creature[i].x, creature[i].y), cvPoint(creature[i].x + (int)3 * range*cos((creature[i].angle + theta)*pi / 180), creature[i].y + 3 * range*sin((creature[i].angle + theta)*pi / 180)), CV_RGB(0, 0, 255));
		cvLine(img, cvPoint(creature[i].x, creature[i].y), cvPoint(creature[i].x + (int)3 * range*cos((creature[i].angle + 2 * theta)*pi / 180), creature[i].y + 3 * range*sin((creature[i].angle + 2 * theta)*pi / 180)), CV_RGB(0, 0, 255));
		cvLine(img, cvPoint(creature[i].x, creature[i].y), cvPoint(creature[i].x + (int)3 * range*cos((creature[i].angle - theta)*pi / 180), creature[i].y + 3 * range*sin((creature[i].angle - theta)*pi / 180)), CV_RGB(0, 0, 255));
		cvLine(img, cvPoint(creature[i].x, creature[i].y), cvPoint(creature[i].x + (int)3 * range*cos((creature[i].angle - 2 * theta)*pi / 180), creature[i].y + 3 * range*sin((creature[i].angle - 2 * theta)*pi / 180)), CV_RGB(0, 0, 255));
	}
	for (i = 0; i < population2; i++) {
		cvEllipse(img, cvPoint(creature2[i].x, creature2[i].y), cvSize(1, 1), 0, 0, 360, CV_RGB(255, 0, 0));
		cvEllipse(img, cvPoint(creature2[i].x, creature2[i].y), cvSize(range2, range2), creature2[i].angle, -2 * theta2, 2 * theta2, CV_RGB(255, 0, 0));
		cvEllipse(img, cvPoint(creature2[i].x, creature2[i].y), cvSize(2 * range2, 2 * range2), creature2[i].angle, -2 * theta2, 2 * theta2, CV_RGB(255, 0, 0));
		cvEllipse(img, cvPoint(creature2[i].x, creature2[i].y), cvSize(3 * range2, 3 * range2), creature2[i].angle, -2 * theta2, 2 * theta2, CV_RGB(255, 0, 0));
		cvLine(img, cvPoint(creature2[i].x, creature2[i].y), cvPoint(creature2[i].x + (int)3 * range2*cos(creature2[i].angle*pi / 180), creature2[i].y + 3 * range2*sin(creature2[i].angle*pi / 180)), CV_RGB(255, 0, 0));
		cvLine(img, cvPoint(creature2[i].x, creature2[i].y), cvPoint(creature2[i].x + (int)3 * range2*cos((creature2[i].angle + theta2)*pi / 180), creature2[i].y + 3 * range2*sin((creature2[i].angle + theta2)*pi / 180)), CV_RGB(255, 0, 0));
		cvLine(img, cvPoint(creature2[i].x, creature2[i].y), cvPoint(creature2[i].x + (int)3 * range2*cos((creature2[i].angle + 2 * theta2)*pi / 180), creature2[i].y + 3 * range2*sin((creature2[i].angle + 2 * theta2)*pi / 180)), CV_RGB(255, 0, 0));
		cvLine(img, cvPoint(creature2[i].x, creature2[i].y), cvPoint(creature2[i].x + (int)3 * range2*cos((creature2[i].angle - theta2)*pi / 180), creature2[i].y + 3 * range2*sin((creature2[i].angle - theta2)*pi / 180)), CV_RGB(255, 0, 0));
		cvLine(img, cvPoint(creature2[i].x, creature2[i].y), cvPoint(creature2[i].x + (int)3 * range2*cos((creature2[i].angle - 2 * theta2)*pi / 180), creature2[i].y + 3 * range2*sin((creature2[i].angle - 2 * theta2)*pi / 180)), CV_RGB(255, 0, 0));
	}

	for (i = 0; i < food_population; i++) {
		cvEllipse(img, cvPoint(food[i].x, food[i].y), cvSize(1, 1), 0, 0, 360, CV_RGB(0, 255, 0), -1);
	}

	cvShowImage("simulation", img);
	cvWaitKey(1);
}

double pesudo_sigmoid(double x) {
	return 2 * (1 / (exp(-x) + 1) - 0.5); //form -1 to 1
}

void initialization(struct object creature[], struct object2 creature2[], struct stuff food[], int population, int population2, int food_population) {

	int i, j, k, l;
	for (i = 0; i < population; i++) {

		for (j = 0; j < NIN + 1; j++) {
			for (k = 0; k < NHN; k++) creature[i].weight_IH[j][k] = (((double)(rand() % 100)) / 100) - 0.5;
		}

		for (j = 0; j < NHL - 1; j++) {
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NHN; l++) creature[i].weight_HH[j][k][l] = (((double)(rand() % 100)) / 100) - 0.5;
			}
		}

		for (j = 0; j < NHN + 1; j++) {
			for (k = 0; k < NON; k++) creature[i].weight_HO[j][k] = (((double)(rand() % 100)) / 100) - 0.5;
		}

		creature[i].starve = initial_starve;
		creature[i].x = rand() % grid;
		creature[i].y = rand() % grid;
		creature[i].angle = (double)(rand() % 360);
		creature[i].on = 1;
	}

	for (i = 0; i < population2; i++) {

		for (j = 0; j < NIN + 1; j++) {
			for (k = 0; k < NHN; k++) creature2[i].weight_IH[j][k] = (((double)(rand() % 100)) / 100) - 0.5;
		}

		for (j = 0; j < NHL - 1; j++) {
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NHN; l++) creature2[i].weight_HH[j][k][l] = (((double)(rand() % 100)) / 100) - 0.5;
			}
		}

		for (j = 0; j < NHN + 1; j++) {
			for (k = 0; k < NON; k++) creature2[i].weight_HO[j][k] = (((double)(rand() % 100)) / 100) - 0.5;
		}

		creature2[i].starve = initial_starve2;
		creature2[i].x = rand() % grid;
		creature2[i].y = rand() % grid;
		creature2[i].angle = (double)(rand() % 360);
		creature2[i].on = 1;
	}
	/* 물체 초기화 완료 */


	/* 음식 초기화 시작 */
	for (i = 0; i < food_population; i++) {
		food[i].x = rand() % grid;
		food[i].y = rand() % grid;
		food[i].on = 1;
	}
	/* 음식 초기화 완료 */
}

void eat_and_reproduce(struct object creature[], struct stuff food[], int * population, int * food_population, struct object ** dummy_creature, struct stuff ** dummy_food) {
	/* eat code start */
	struct object * tempt_creature;
	struct stuff * tempt_food;
	double distance;
	double delx;
	double dely;
	int count = 0;
	int tempt = 0;
	int surplus;

	int i, j, k, l, m, n;

	for (i = 0; i < *population; i++) {
		for (j = 0; j < *food_population; j++) {
			delx = creature[i].x - food[j].x;
			dely = creature[i].y - food[j].y;
			distance = sqrt(delx*delx + dely*dely);

			if (distance <= eat_distance) {
				if (food[j].on == 1) {
					creature[i].starve = creature[i].starve + foodplus;
					creature[i].on = creature[i].on + 1;
					food[j].on = 0;
					count = count + 1;
				}
			}
		}
	}
	tempt = *food_population;
	*food_population = *food_population - count;
	tempt_food = (struct stuff *)malloc(sizeof(struct stuff)*(*food_population));
	j = 0;
	for (i = 0; i < tempt; i++) {
		if (food[i].on == 1) {
			tempt_food[j].x = food[i].x;
			tempt_food[j].y = food[i].y;
			tempt_food[j].on = food[i].on;
			j = j + 1;
		}
	}
	free(food);
	food = (struct stuff *)malloc(sizeof(struct stuff)*(*food_population));
	for (i = 0; i < *food_population; i++) {
		food[i].x = tempt_food[i].x;
		food[i].y = tempt_food[i].y;
		food[i].on = tempt_food[i].on;
	}
	free(tempt_food);

	tempt = *population;
	*population = *population + count;
	tempt_creature = (struct object *)malloc(sizeof(struct object)*(*population));
	j = 0; k = 0;
	for (i = 0; i < tempt; i++) {
		if (creature[i].on > 1) {
			surplus = creature[i].on - 1;
			/*original copy start*/
			tempt_creature[j].x = creature[i].x;
			tempt_creature[j].y = creature[i].y;
			tempt_creature[j].angle = creature[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature[j].weight_IH[k][l] = creature[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature[j].weight_HH[k][l][m] = creature[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature[j].weight_HO[k][l] = creature[i].weight_HO[k][l];
			}

			tempt_creature[j].starve = creature[i].starve;
			tempt_creature[j].on = 1;

			j = j + 1;
			/*original copy end*/
			for (n = 0; n < surplus; n++) {
				tempt_creature[j].x = creature[i].x;
				tempt_creature[j].y = creature[i].y;
				tempt_creature[j].angle = creature[i].angle;

				for (k = 0; k < NIN + 1; k++) {
					for (l = 0; l < NHN; l++) tempt_creature[j].weight_IH[k][l] = creature[i].weight_IH[k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}
				for (k = 0; k < NHL - 1; k++) {
					for (l = 0; l < NHN + 1; l++) {
						for (m = 0; m < NHN; m++) tempt_creature[j].weight_HH[k][l][m] = creature[i].weight_HH[k][l][m] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
					}
				}
				for (k = 0; k < NHN + 1; k++) {
					for (l = 0; l < NON; l++) tempt_creature[j].weight_HO[k][l] = creature[i].weight_HO[k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}

				tempt_creature[j].starve = initial_starve;
				tempt_creature[j].on = 1;

				j = j + 1;
			}
		}
		else {
			tempt_creature[j].x = creature[i].x;
			tempt_creature[j].y = creature[i].y;
			tempt_creature[j].angle = creature[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature[j].weight_IH[k][l] = creature[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature[j].weight_HH[k][l][m] = creature[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature[j].weight_HO[k][l] = creature[i].weight_HO[k][l];
			}

			tempt_creature[j].starve = creature[i].starve;
			tempt_creature[j].on = creature[i].on;

			j = j + 1;
		}
	}
	free(creature);
	creature = (struct object *)malloc(sizeof(struct object)*(*population));
	for (i = 0; i < *population; i++) {
		creature[i].x = tempt_creature[i].x;
		creature[i].y = tempt_creature[i].y;
		creature[i].angle = tempt_creature[i].angle;

		for (k = 0; k < NIN + 1; k++) {
			for (l = 0; l < NHN; l++) creature[i].weight_IH[k][l] = tempt_creature[i].weight_IH[k][l];
		}
		for (k = 0; k < NHL - 1; k++) {
			for (l = 0; l < NHN + 1; l++) {
				for (m = 0; m < NHN; m++) creature[i].weight_HH[k][l][m] = tempt_creature[i].weight_HH[k][l][m];
			}
		}
		for (k = 0; k < NHN + 1; k++) {
			for (l = 0; l < NON; l++) creature[i].weight_HO[k][l] = tempt_creature[i].weight_HO[k][l];
		}

		creature[i].starve = tempt_creature[i].starve;
		creature[i].on = tempt_creature[i].on;
	}
	free(tempt_creature);
	/* eat code end */
	*dummy_creature = creature;
	*dummy_food = food;
}

void death(struct object creature[], int* population, struct object ** dummy_creature) {
	/*death code start*/
	struct object * tempt_creature;
	int count = 0;
	int tempt = 0;
	int i, j, k, l, m;
	for (i = 0; i < *population; i++) {
		if (creature[i].starve <= 0) {
			creature[i].on = 0;
			count = count + 1;
		}
	}
	tempt = *population;
	*population = *population - count;
	tempt_creature = (struct object *)malloc(sizeof(struct object)*(*population));
	j = 0;
	for (i = 0; i < tempt; i++) {
		if (creature[i].on == 1) {
			tempt_creature[j].x = creature[i].x;
			tempt_creature[j].y = creature[i].y;
			tempt_creature[j].angle = creature[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature[j].weight_IH[k][l] = creature[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature[j].weight_HH[k][l][m] = creature[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature[j].weight_HO[k][l] = creature[i].weight_HO[k][l];
			}

			tempt_creature[j].starve = creature[i].starve;
			tempt_creature[j].on = creature[i].on;

			j = j + 1;
		}
	}
	free(creature);
	creature = (struct object *)malloc(sizeof(struct object)*(*population));
	for (i = 0; i < *population; i++) {
		creature[i].x = tempt_creature[i].x;
		creature[i].y = tempt_creature[i].y;
		creature[i].angle = tempt_creature[i].angle;

		for (k = 0; k < NIN + 1; k++) {
			for (l = 0; l < NHN; l++) creature[i].weight_IH[k][l] = tempt_creature[i].weight_IH[k][l];
		}
		for (k = 0; k < NHL - 1; k++) {
			for (l = 0; l < NHN + 1; l++) {
				for (m = 0; m < NHN; m++) creature[i].weight_HH[k][l][m] = tempt_creature[i].weight_HH[k][l][m];
			}
		}
		for (k = 0; k < NHN + 1; k++) {
			for (l = 0; l < NON; l++) creature[i].weight_HO[k][l] = tempt_creature[i].weight_HO[k][l];
		}

		creature[i].starve = tempt_creature[i].starve;
		creature[i].on = tempt_creature[i].on;
	}
	free(tempt_creature);
	/*death code end*/
	*dummy_creature = creature;
}

void death2(struct object2 creature2[], int* population2, struct object2 ** dummy_creature2) {
	/*death code start*/
	struct object2 * tempt_creature2;
	int count = 0;
	int tempt = 0;
	int i, j, k, l, m;
	for (i = 0; i < *population2; i++) {
		if (creature2[i].starve <= 0) {
			creature2[i].on = 0;
			count = count + 1;
		}
	}
	tempt = *population2;
	*population2 = *population2 - count;
	tempt_creature2 = (struct object2 *)malloc(sizeof(struct object2)*(*population2));
	j = 0;
	for (i = 0; i < tempt; i++) {
		if (creature2[i].on == 1) {
			tempt_creature2[j].x = creature2[i].x;
			tempt_creature2[j].y = creature2[i].y;
			tempt_creature2[j].angle = creature2[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature2[j].weight_IH[k][l] = creature2[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature2[j].weight_HH[k][l][m] = creature2[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature2[j].weight_HO[k][l] = creature2[i].weight_HO[k][l];
			}

			tempt_creature2[j].starve = creature2[i].starve;
			tempt_creature2[j].on = creature2[i].on;

			j = j + 1;
		}
	}
	free(creature2);
	creature2 = (struct object2 *)malloc(sizeof(struct object2)*(*population2));
	for (i = 0; i < *population2; i++) {
		creature2[i].x = tempt_creature2[i].x;
		creature2[i].y = tempt_creature2[i].y;
		creature2[i].angle = tempt_creature2[i].angle;

		for (k = 0; k < NIN + 1; k++) {
			for (l = 0; l < NHN; l++) creature2[i].weight_IH[k][l] = tempt_creature2[i].weight_IH[k][l];
		}
		for (k = 0; k < NHL - 1; k++) {
			for (l = 0; l < NHN + 1; l++) {
				for (m = 0; m < NHN; m++) creature2[i].weight_HH[k][l][m] = tempt_creature2[i].weight_HH[k][l][m];
			}
		}
		for (k = 0; k < NHN + 1; k++) {
			for (l = 0; l < NON; l++) creature2[i].weight_HO[k][l] = tempt_creature2[i].weight_HO[k][l];
		}

		creature2[i].starve = tempt_creature2[i].starve;
		creature2[i].on = tempt_creature2[i].on;
	}
	free(tempt_creature2);
	/*death code end*/
	*dummy_creature2 = creature2;
}

void create_food(struct stuff food[], int *food_population, struct stuff ** dummy_food) {
	int i;
	struct stuff * tempt_food;
	int tempt;
	int probability;
	/*create food start*/
	if (*food_population < food_limit) {
		probability = (int)(food_create_probability * 100);
		if (rand() % 100 < probability) {
			tempt = 0;
			tempt = *food_population;
			*food_population = *food_population + createfood;
			tempt_food = (struct stuff *)malloc(sizeof(struct stuff)*(*food_population));
			for (i = 0; i < tempt; i++) {
				tempt_food[i].x = food[i].x;
				tempt_food[i].y = food[i].y;
				tempt_food[i].on = food[i].on;
			}
			tempt_food[tempt].x = (double)(rand() % grid);
			tempt_food[tempt].y = (double)(rand() % grid);
			tempt_food[tempt].on = 1;

			free(food);
			food = (struct stuff *)malloc(sizeof(struct stuff)*(*food_population));
			for (i = 0; i < *food_population; i++) {
				food[i].x = tempt_food[i].x;
				food[i].y = tempt_food[i].y;
				food[i].on = tempt_food[i].on;
			}
			free(tempt_food);
		}
	}
	/*create food end*/
	*dummy_food = food;
}

void eat_and_reproduce2(struct object creature[], struct object2 creature2[], int * population, int * population2, struct object2 ** dummy_creature2, struct object ** dummy_creature) {
	/* eat code start */
	struct object * dummy;
	struct object * tempt_creature;
	struct object2 * tempt_creature2;
	double distance;
	double delx;
	double dely;
	int count = 0;
	int tempt = 0;
	int surplus;

	int i, j, k, l, m, n;

	for (i = 0; i < *population2; i++) {
		for (j = 0; j < *population; j++) {
			delx = creature2[i].x - creature[j].x;
			dely = creature2[i].y - creature[j].y;
			distance = sqrt(delx*delx + dely*dely);

			if (distance <= eat_distance) {
				if (creature[j].on == 1) {
					creature2[i].starve = creature2[i].starve + foodplus2;
					creature2[i].on = creature2[i].on + 1;
					creature[j].on = 0;
					count = count + 1;
				}
			}
		}
	}




	tempt = *population;
	*population = *population - count;
	tempt_creature = (struct object *)malloc(sizeof(struct object)*(*population));
	j = 0;
	for (i = 0; i < tempt; i++) {
		if (creature[i].on == 1) {
			tempt_creature[j].x = creature[i].x;
			tempt_creature[j].y = creature[i].y;
			tempt_creature[j].angle = creature[i].angle;
			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature[j].weight_IH[k][l] = creature[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature[j].weight_HH[k][l][m] = creature[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature[j].weight_HO[k][l] = creature[i].weight_HO[k][l];
			}
			tempt_creature[j].starve = creature[i].starve;
			tempt_creature[j].on = creature[i].on;
			j = j + 1;
		}
	}

	free(creature);
	creature = (struct object *)malloc(sizeof(struct object)*(*population));
	for (i = 0; i < *population; i++) {
		creature[i].x = tempt_creature[i].x;
		creature[i].y = tempt_creature[i].y;
		creature[i].angle = tempt_creature[i].angle;
		for (k = 0; k < NIN + 1; k++) {
			for (l = 0; l < NHN; l++) creature[i].weight_IH[k][l] = tempt_creature[i].weight_IH[k][l];
		}
		for (k = 0; k < NHL - 1; k++) {
			for (l = 0; l < NHN + 1; l++) {
				for (m = 0; m < NHN; m++) creature[i].weight_HH[k][l][m] = tempt_creature[i].weight_HH[k][l][m];
			}
		}
		for (k = 0; k < NHN + 1; k++) {
			for (l = 0; l < NON; l++) creature[i].weight_HO[k][l] = tempt_creature[i].weight_HO[k][l];
		}

		creature[i].starve = tempt_creature[i].starve;
		creature[i].on = tempt_creature[i].on;
	}
	free(tempt_creature);





	tempt = *population2;
	*population2 = *population2 + count;
	tempt_creature2 = (struct object2 *)malloc(sizeof(struct object2)*(*population2));
	j = 0; k = 0;
	for (i = 0; i < tempt; i++) {
		if (creature2[i].on > 1) {
			surplus = creature2[i].on - 1;
			/*original copy start*/
			tempt_creature2[j].x = creature2[i].x;
			tempt_creature2[j].y = creature2[i].y;
			tempt_creature2[j].angle = creature2[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature2[j].weight_IH[k][l] = creature2[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature2[j].weight_HH[k][l][m] = creature2[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature2[j].weight_HO[k][l] = creature2[i].weight_HO[k][l];
			}

			tempt_creature2[j].starve = creature2[i].starve;
			tempt_creature2[j].on = 1;

			j = j + 1;
			/*original copy end*/
			for (n = 0; n < surplus; n++) {
				tempt_creature2[j].x = creature2[i].x;
				tempt_creature2[j].y = creature2[i].y;
				tempt_creature2[j].angle = creature2[i].angle;

				for (k = 0; k < NIN + 1; k++) {
					for (l = 0; l < NHN; l++) tempt_creature2[j].weight_IH[k][l] = creature2[i].weight_IH[k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}
				for (k = 0; k < NHL - 1; k++) {
					for (l = 0; l < NHN + 1; l++) {
						for (m = 0; m < NHN; m++) tempt_creature2[j].weight_HH[k][l][m] = creature2[i].weight_HH[k][l][m] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
					}
				}
				for (k = 0; k < NHN + 1; k++) {
					for (l = 0; l < NON; l++) tempt_creature2[j].weight_HO[k][l] = creature2[i].weight_HO[k][l] + (((float)(rand() % 100)) / 100 - 0.5)*mutation_rate;
				}

				tempt_creature2[j].starve = initial_starve2;
				tempt_creature2[j].on = 1;

				j = j + 1;
			}
		}
		else {
			tempt_creature2[j].x = creature2[i].x;
			tempt_creature2[j].y = creature2[i].y;
			tempt_creature2[j].angle = creature2[i].angle;

			for (k = 0; k < NIN + 1; k++) {
				for (l = 0; l < NHN; l++) tempt_creature2[j].weight_IH[k][l] = creature2[i].weight_IH[k][l];
			}
			for (k = 0; k < NHL - 1; k++) {
				for (l = 0; l < NHN + 1; l++) {
					for (m = 0; m < NHN; m++) tempt_creature2[j].weight_HH[k][l][m] = creature2[i].weight_HH[k][l][m];
				}
			}
			for (k = 0; k < NHN + 1; k++) {
				for (l = 0; l < NON; l++) tempt_creature2[j].weight_HO[k][l] = creature2[i].weight_HO[k][l];
			}

			tempt_creature2[j].starve = creature2[i].starve;
			tempt_creature2[j].on = creature2[i].on;

			j = j + 1;
		}
	}
	free(creature2);
	creature2 = (struct object2 *)malloc(sizeof(struct object2)*(*population2));
	for (i = 0; i < *population2; i++) {
		creature2[i].x = tempt_creature2[i].x;
		creature2[i].y = tempt_creature2[i].y;
		creature2[i].angle = tempt_creature2[i].angle;

		for (k = 0; k < NIN + 1; k++) {
			for (l = 0; l < NHN; l++) creature2[i].weight_IH[k][l] = tempt_creature2[i].weight_IH[k][l];
		}
		for (k = 0; k < NHL - 1; k++) {
			for (l = 0; l < NHN + 1; l++) {
				for (m = 0; m < NHN; m++) creature2[i].weight_HH[k][l][m] = tempt_creature2[i].weight_HH[k][l][m];
			}
		}
		for (k = 0; k < NHN + 1; k++) {
			for (l = 0; l < NON; l++) creature2[i].weight_HO[k][l] = tempt_creature2[i].weight_HO[k][l];
		}

		creature2[i].starve = tempt_creature2[i].starve;
		creature2[i].on = tempt_creature2[i].on;
	}
	free(tempt_creature2);
	/* eat code end */
	*dummy_creature = creature;
	*dummy_creature2 = creature2;

}

int rounding(double num) {
	return (int)(floor(num + 0.5));
}