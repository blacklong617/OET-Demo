#include "stdafx.h"
//#include <vector>
#include "sdca.h"
#include <math.h>
#include<algorithm>
using namespace std;

vector<CellData> datalist;      //原始数据数组
int loadData(const char* filename){
	vector<CellData> original_data;
	CellData intp;
	FILE *fp;
	fp = fopen(filename,"r");
	if (fp == NULL) {
		return OPEN_DATA_ERROR;
	}
	int i = 0;
	while (1) {
		fscanf(fp, "%d	%f	%f	%s	%s	%s\n", &intp.num, &intp.z1, &intp.z2, &intp.lat, &intp.lon, &intp.time);
		intp.num = i;
		original_data.push_back(intp);
		//cout << i << " " << incsv[i].z1 << " " << incsv[i].z2 << " " << incsv[i].lat << " " << incsv[i].lon<<" " << incsv[i].time << " " << endl;
		i++;
		if (feof(fp))break;
	}
	fclose(fp);
	if (original_data.size() <= 0) {
		return LOAD_DATA_ERROR;
	}
	datalist =cutData(original_data);
	return SUCCESS;
}
int findFunPoint(vector<CellData> vecdata, vector<fun_point> &fun_points) {
	vector <CellData> element;
	float sq1 = 0;
	float sq2 = 0;
	float sh1 = 0;
	float sh2 = 0;

	fun_point point;//兴趣点

	//查找兴趣点存放于fun_points 宽度为20的滑条，前后滑块距离为10，通过计算前后滑块面差判断是否为边界点
	for (int i = 0; i + 19<vecdata.size(); i = i + 10)//size()容器中实际数据个数 
	{
		element.assign(vecdata.begin() + i, vecdata.begin() + i + 20);
		//printf("element debug!\n");
		/*分析element的10组数据*/
		sh1 = 0;
		sh2 = 0;
		for (auto it = element.begin(); it != element.end(); it++) {

			sh1 = sh1 + (*it).z1;
			sh2 = sh2 + (*it).z2;
		}
		sh1 = sh1 / 20;
		sh2 = sh2 / 20;
		/*
		*###sh1-sq1 sh2-sq2：一条线前后差
		*###sq2 - sq1：两条线前差
		*###sh2 - sh1：两条线后差
		*/
		if (i == 0) {
			sq1 = sh1;
			sq2 = sh2;
		}
		else {

			if ((abs(sh1 - sq1) >= 5) && (abs(sh2 - sq2) <= 1) && (abs(sq2 - sq1) < 10) && (abs(sh2 - sh1) > 1)) {
				if ((sh1 + sh2 + sq1 + sq2) >= 950) {
#ifdef DEBUG
					printf("start1 i=%d", element[1].num);
#endif
					//start1.push_back(i);
					//findTP.push_back(i);
					//存入兴趣点
					point.num = element[1].num;
					strcpy(point.lon, element[1].lon);
					strcpy(point.lat, element[1].lat);
					strcpy(point.time, element[1].time);
					point.lineID = 1;
					point.point_class = 1;
					point.sh1 = sh1;
					point.sq1 = sq1;
					point.sh2 = sh2;
					point.sq2 = sq2;
					fun_points.push_back(point);
#ifdef DEBUG
					printf(" sq1=%f,sh1=%f,sq2=%f,sh2=%f\n", sq1, sh1, sq2, sh2);
#endif
				}
			}//(abs(sh2 - sh1) < 1) &&   && (abs(sh2 - sh1) < 1)
			else if ((abs(sh1 - sq1) >= 5) && (abs(sh2 - sq2) <= 1) && (abs(sh2 - sh1) < 10) && (abs(sq2 - sq1) > 1)) {
				if ((sh1 + sh2 + sq1 + sq2) >= 950) {
					/*
					*###sh1-sq1 sh2-sq2：一条线前后差
					*###sq2 - sq1：两条线前差
					*###sh2 - sh1：两条线后差
					*/
					//存入兴趣点
					point.num = element[1].num;
					//point.lon = element[1].lon;
					strcpy(point.lon, element[1].lon);
					strcpy(point.lat, element[1].lat);
					strcpy(point.time, element[1].time);
					point.lineID = 1;
					point.point_class = 2;
					point.sh1 = sh1;
					point.sq1 = sq1;
					point.sh2 = sh2;
					point.sq2 = sq2;
					fun_points.push_back(point);
#ifdef DEBUG
					printf("end1   i=%d", element[1].num);
					printf(" sq1=%f,sh1=%f,sq2=%f,sh2=%f\n", sq1, sh1, sq2, sh2);
#endif
				}
			}
			else if ((abs(sh2 - sq2) >= 5) && (abs(sh1 - sq1) <= 1) && (abs(sq2 - sq1) < 10) && (abs(sh2 - sh1) > 1)) {
				if ((sh1 + sh2 + sq1 + sq2) >= 950) {
#ifdef DEBUG
					printf("start2 i=%d", element[1].num);
#endif
					//存入兴趣点
					point.num = element[1].num;
					strcpy(point.lon, element[1].lon);
					strcpy(point.lat, element[1].lat);
					strcpy(point.time, element[1].time);
					point.lineID = 2;
					point.point_class = 1;
					point.sh1 = sh1;
					point.sq1 = sq1;
					point.sh2 = sh2;
					point.sq2 = sq2;
					fun_points.push_back(point);
#ifdef DEBUG
					printf(" sq1=%f,sh1=%f,sq2=%f,sh2=%f\n", sq1, sh1, sq2, sh2);
#endif
				}

			}
			else if ((abs(sh2 - sq2) >= 5) && (abs(sh1 - sq1) <= 1) && (abs(sq2 - sq1) > 1) && (abs(sh2 - sh1) < 10)) {
				if ((sh1 + sh2 + sq1 + sq2) >= 950) {
#ifdef DEBUG
					printf("end2   i=%d", element[1].num);
#endif

					//存入兴趣点
					point.num = element[1].num;
					strcpy(point.lon, element[1].lon);
					strcpy(point.lat, element[1].lat);
					strcpy(point.time, element[1].time);
					point.lineID = 2;
					point.point_class = 2;
					point.sh1 = sh1;
					point.sq1 = sq1;
					point.sh2 = sh2;
					point.sq2 = sq2;
					fun_points.push_back(point);
#ifdef DEBUG
					printf(" sq1=%f,sh1=%f,sq2=%f,sh2=%f\n", sq1, sh1, sq2, sh2);
#endif
				}
			}
			sq1 = sh1;
			sq2 = sh2;

		}

	}
	if (fun_points.size() <= 0) {
		return SD_CALCULATE_ERROR;
	}
	return SUCCESS;
}

/**处理得到的兴趣点
**NEED UPDATE!!!
**对各种情况的兴趣点进行分析（前提，数据已去除掉非轨面部分）
**情况一：数据为start-end-end型
**情况二：数据为start-start-end型
**情况三：数据为start-end-start型，不处理
**输入参数：ex_points、vecdata、resultpoint指针（引用）
**输出参数：resultpoint、ex_points
**/
int handleFunPoint(vector<fun_point> &fun_points) {
	//情况一：
	for (int i = 0; i + 2<fun_points.size(); i++) {

		if (fun_points[i].point_class == 1 && fun_points[i + 1].point_class == 2 && fun_points[i + 2].point_class == 2) {
			//fun_points.erase(fun_points.begin() + i + 1);
			if ((fun_points[i + 1].num - fun_points[i + 2].num) < 300 &&
				fun_points[i].lineID == fun_points[i + 1].lineID&&fun_points[i + 1].lineID == fun_points[i + 2].lineID) {
#ifdef DEBUG
				printf("end-end distance less than 300*0.17mm!!\n");
#endif
				fun_points.erase(fun_points.begin() + i + 1);
			}

		}

	}
	//情况二
	for (int i = 0; i + 2<fun_points.size(); i++) {

		if (fun_points[i].point_class == 1 && fun_points[i + 1].point_class == 1 && fun_points[i + 2].point_class == 2) {
			//fun_points.erase(fun_points.begin() + i + 1);
			if ((fun_points[i].num - fun_points[i + 1].num) < 300 &&
				fun_points[i].lineID == fun_points[i + 1].lineID&&fun_points[i + 1].lineID == fun_points[i + 2].lineID) {
#ifdef DEBUG
				printf("start-start distance less than 300*0.17mm!!\n");
#endif
				fun_points.erase(fun_points.begin() + i + 1);
			}

		}

	}
	//情况三
	for (int i = 0; i + 1 < fun_points.size(); i++) {

		if (fun_points[i].point_class == 1 && fun_points[i + 1].point_class == 2) {
			if (abs(fun_points[i + 1].num - fun_points[i].num) < 100 &&
				fun_points[i].lineID == fun_points[i + 1].lineID) {
#ifdef DEBUG
				printf("start-end distance less than 100*0.17mm!!\n");
#endif

			}
		}

	}
	return 0;

}

/**
**判定类别、是否合格等
**输入参数：fun_points
**输出参数：ex_points、Result_Points
**/
int judgmentItem(vector<CellData> vecdata,
	vector<fun_point> &fun_points, 
	vector<fun_point> &ex_points, 
	vector<DetectPoint> &Result_Points) {
	//分组合并方案

	float sdq = 0;//检测项前面深度值
	float sdh = 0;//检测项后面深度值
	int dn = 1;
				  //printf("start sd\n");
	for (int i = 0; i < fun_points.size() - 1; i++) {
		DetectPoint resultpoint;
		//resultpoint.loc_num = i;
		resultpoint.latitude = fun_points[i].lat;
		resultpoint.longitude = fun_points[i].lon;
		//printf("num=%d,lineID=%d,point_class=%d,sq1=%f,sh1=%f,sq2=%f,sh2=%f\n",
		//	fun_points[i].num, fun_points[i].lineID, fun_points[i].point_class, fun_points[i].sq1, fun_points[i].sh1, fun_points[i].sq2, fun_points[i].sh2);
		if (fun_points[i].point_class == 1 && fun_points[i + 1].point_class == 2 &&
			fun_points[i].lineID == 2 && fun_points[i + 1].lineID == 2) {

			if (abs(fun_points[i].num - fun_points[i + 1].num) <= 100) {
				for (int j = fun_points[i].num - 151; j + 50 < fun_points[i].num; j++) {
					sdq = sdq + vecdata[j].z2;
				}
				//printf("start cal sdh!\n");
				for (int k = fun_points[i + 1].num + 50; k < fun_points[i + 1].num + 151; k++) {
					sdh = sdh + vecdata[k].z2;
				}
				resultpoint.type = CONNECTOR_JOINT;
				resultpoint.stand_value = 0.2;
			}
			else {
				for (int j = fun_points[i].num - 301; j + 100 < fun_points[i].num; j++) {
					sdq = sdq + vecdata[j].z2;
				}
				//printf("start cal sdh!\n");
				for (int k = fun_points[i + 1].num + 100; k < fun_points[i + 1].num + 301; k++) {
					sdh = sdh + vecdata[k].z2;
				}
				resultpoint.type = EXPAND_JOINT;
				resultpoint.stand_value = 0.5;
			}
			ex_points.push_back(fun_points[i]);
			ex_points.push_back(fun_points[i + 1]);
			i++;
		}
		else if (fun_points[i].point_class == 1 && fun_points[i + 1].point_class == 2 &&
			fun_points[i].lineID == 1 && fun_points[i + 1].lineID == 1) {

			if (abs(fun_points[i].num - fun_points[i + 1].num) <= 100) {
				for (int j = fun_points[i].num - 151; j + 50 < fun_points[i].num; j++) {
					sdq = sdq + vecdata[j].z1;
				}
				//printf("start cal sdh!\n");
				for (int k = fun_points[i + 1].num + 50; k< fun_points[i + 1].num + 151; k++) {
					sdh = sdh + vecdata[k].z1;
				}
				resultpoint.type = CONNECTOR_JOINT;
				resultpoint.stand_value = 0.2;
			}
			else {
				for (int j = fun_points[i].num - 301; j + 100 < fun_points[i].num; j++) {
					sdq = sdq + vecdata[j].z1;
				}
				//printf("start cal sdh!\n");
				for (int k = fun_points[i + 1].num + 100; k< fun_points[i + 1].num + 301; k++) {
					sdh = sdh + vecdata[k].z1;
				}
				resultpoint.type = EXPAND_JOINT;
				resultpoint.stand_value = 0.5;
			}
			ex_points.push_back(fun_points[i]);
			ex_points.push_back(fun_points[i + 1]);
			i++;
		}
		if (resultpoint.type == EXPAND_JOINT) {
			sdq = sdq / 200;
			sdh = sdh / 200;
		}
		else {
			sdq = sdq / 100;
			sdh = sdh / 100;
		}
		resultpoint.loc_num = dn;
		dn=dn + 1;
		resultpoint.measure_value = abs(sdq - sdh);
		if (resultpoint.measure_value <= resultpoint.stand_value) {
			resultpoint.qualified = true;
		}
		else {
			resultpoint.qualified = false;
		}
		Result_Points.push_back(resultpoint);
#ifdef DEBUG
		printf("sdq=%f,sdh=%f,type==%d,measure_value=%f,qualified==%d,lon==%s\n",
			sdq, sdh, resultpoint.type, resultpoint.measure_value, resultpoint.qualified, resultpoint.longitude.c_str());
#endif
		sdq = 0;
		sdh = 0;

	}
	return 0;
}
//扫描异常点
//扫描点前100个点均值与后100个点均值的面差
//计算参数
//输入参数：ex_points、vecdata、resultpoint指针（引用）
//输出参数：resultpoint、ex_points
int scanOddPoint(vector<CellData> vecdata, vector<fun_point> &ex_points, vector<DetectPoint> &resultpoint) {
	float threshold_mean = 0.2;//均值带前后阈值 阈值越大要求前后面相差越大。
	float threshold_variance = 0.125;//均值带方差和阈值 (1/2*threshold_mean)*(1/2*threshold_mean)*sd 阈值越小，要求前后越平整。
	int sd = 50;//均值带
	int ed = 5;//缓冲带

	float sum_z1_q = 0;
	float sum_z1_h = 0;
	float sum_z2_q = 0;
	float sum_z2_h = 0;
	float mean_z1_q = 0;
	float mean_z1_h = 0;
	float mean_z2_q = 0;
	float mean_z2_h = 0;
	float variance_z1_q = 0;
	float variance_z1_h = 0;
	float variance_z2_q = 0;
	float variance_z2_h = 0;
	DetectPoint dp;
	for (int i = 0; i + 2 <= ex_points.size(); i++) {
		//第一个区域
		if (i == 0) {
			for (int s = sd + ed; s + sd + ed + 1 < ex_points[i].num; s = s + 20) {
				//float sum = 0;
				sum_z1_q = 0;
				sum_z1_h = 0;
				sum_z2_q = 0;
				sum_z2_h = 0;
				mean_z1_q = 0;
				mean_z1_h = 0;
				mean_z2_q = 0;
				mean_z2_h = 0;
				variance_z1_q = 0;
				variance_z1_h = 0;
				variance_z2_q = 0;
				variance_z2_h = 0;
				for (int c = s - (sd + ed); c < s - ed; c++) {
					sum_z1_q = sum_z1_q + vecdata[c].z1;
					sum_z2_q = sum_z2_q + vecdata[c].z2;
				}
				for (int c = s + ed; c < s + sd + ed; c++) {
					sum_z1_h = sum_z1_h + vecdata[c].z1;
					sum_z2_h = sum_z2_h + vecdata[c].z2;
				}
				mean_z1_q = sum_z1_q / sd;
				mean_z1_h = sum_z1_h / sd;
				mean_z2_q = sum_z2_q / sd;
				mean_z2_h = sum_z2_h / sd;
				if (abs(mean_z1_q - mean_z1_h) >= threshold_mean &&abs(mean_z2_q - mean_z2_h) >= threshold_mean) {
					for (int c = s - (sd + ed); c < s - ed; c++) {
						variance_z1_q = variance_z1_q + pow(abs(vecdata[c].z1 - mean_z1_q), 2);
						variance_z2_q = variance_z2_q + pow(abs(vecdata[c].z2 - mean_z2_q), 2);
					}
					for (int c = s + ed; c < s + sd + ed; c++) {
						variance_z1_h = variance_z1_h + pow(abs(vecdata[c].z1 - mean_z1_h), 2);
						variance_z2_h = variance_z2_h + pow(abs(vecdata[c].z2 - mean_z2_h), 2);
					}
					if (variance_z1_q < threshold_variance&&variance_z2_q < threshold_variance&&variance_z1_h < threshold_variance&&variance_z2_h < threshold_variance) {
#ifdef DEBUG
						printf("variance_z1_q=%f,variance_z1_h=%f,variance_z2_q=%f,variance_z2_h=%f\n", 
							variance_z1_q, variance_z1_h, variance_z2_q, variance_z2_h);
						printf("sss==%d\n", s);
#endif // DEBUG
						//添加s至resultpoint
						dp.loc_num = resultpoint.size() + 1;
						dp.type = CONNECTOR_JOINT;
						dp.latitude = vecdata[s].lat;
						dp.longitude = vecdata[s].lon;
						dp.stand_value = 0.2;
						dp.measure_value = (abs(mean_z1_q - mean_z1_h) + abs(mean_z2_q - mean_z2_h))*0.5;
						dp.qualified = false;
						resultpoint.push_back(dp);


						//resultpoint.push_back();
					}

					s = s + 199;
				}

			}
		}
		else if (i == ex_points.size() - 1) {//最后一个区域
			for (int s = ex_points[i].num + sd + ed; s + sd + ed + 1 < vecdata.size() - 1; s = s + 20) {
				//float sum = 0;
				sum_z1_q = 0;
				sum_z1_h = 0;
				sum_z2_q = 0;
				sum_z2_h = 0;
				mean_z1_q = 0;
				mean_z1_h = 0;
				mean_z2_q = 0;
				mean_z2_h = 0;
				variance_z1_q = 0;
				variance_z1_h = 0;
				variance_z2_q = 0;
				variance_z2_h = 0;
				for (int c = s - (sd + ed); c < s - ed; c++) {
					sum_z1_q = sum_z1_q + vecdata[c].z1;
					sum_z2_q = sum_z2_q + vecdata[c].z2;
				}
				for (int c = s + ed; c < s + ed + sd; c++) {
					sum_z1_h = sum_z1_h + vecdata[c].z1;
					sum_z2_h = sum_z2_h + vecdata[c].z2;
				}
				mean_z1_q = sum_z1_q / sd;
				mean_z1_h = sum_z1_h / sd;
				mean_z2_q = sum_z2_q / sd;
				mean_z2_h = sum_z2_h / sd;
				if (abs(mean_z1_q - mean_z1_h) >= threshold_mean &&abs(mean_z2_q - mean_z2_h) >= threshold_mean) {
					for (int c = s - (sd + ed); c < s - ed; c++) {
						variance_z1_q = variance_z1_q + pow(abs(vecdata[c].z1 - mean_z1_q), 2);
						variance_z2_q = variance_z2_q + pow(abs(vecdata[c].z2 - mean_z2_q), 2);
					}
					for (int c = s + ed; c < s + ed + sd; c++) {
						variance_z1_h = variance_z1_h + pow(abs(vecdata[c].z1 - mean_z1_h), 2);
						variance_z2_h = variance_z2_h + pow(abs(vecdata[c].z2 - mean_z2_h), 2);
					}
					if (variance_z1_q < threshold_variance&&variance_z2_q < threshold_variance &&variance_z1_h < threshold_variance&&variance_z2_h < threshold_variance) {
#ifdef DEBUG
						printf("variance_z1_q=%f,variance_z1_h=%f,variance_z2_q=%f,variance_z2_h=%f\n",
							variance_z1_q, variance_z1_h, variance_z2_q, variance_z2_h);
						printf("sss==%d\n", s);
#endif // DEBUG
						//添加s至resultpoint
						dp.loc_num = resultpoint.size() + 1;
						dp.type = CONNECTOR_JOINT;
						dp.latitude = vecdata[s].lat;
						dp.longitude = vecdata[s].lon;
						dp.stand_value = 0.2;
						dp.measure_value = (abs(mean_z1_q - mean_z1_h) + abs(mean_z2_q - mean_z2_h))*0.5;
						dp.qualified = false;
						resultpoint.push_back(dp);

					}
					s = s + 199;
				}

			}

		}
		else {//其他区域
			for (int s = ex_points[i].num + ed + sd; s + ed + sd + 1 < ex_points[i + 1].num; s++) {
				sum_z1_q = 0;
				sum_z1_h = 0;
				sum_z2_q = 0;
				sum_z2_h = 0;
				mean_z1_q = 0;
				mean_z1_h = 0;
				mean_z2_q = 0;
				mean_z2_h = 0;
				variance_z1_q = 0;
				variance_z1_h = 0;
				variance_z2_q = 0;
				variance_z2_h = 0;
				for (int c = s - (sd + ed); c < s - ed; c++) {
					sum_z1_q = sum_z1_q + vecdata[c].z1;
					sum_z2_q = sum_z2_q + vecdata[c].z2;
				}
				for (int c = s + ed; c < s + sd + ed; c++) {
					sum_z1_h = sum_z1_h + vecdata[c].z1;
					sum_z2_h = sum_z2_h + vecdata[c].z2;
				}
				mean_z1_q = sum_z1_q / sd;
				mean_z1_h = sum_z1_h / sd;
				mean_z2_q = sum_z2_q / sd;
				mean_z2_h = sum_z2_h / sd;
				if (abs(mean_z1_q - mean_z1_h) >= threshold_mean &&abs(mean_z2_q - mean_z2_h) >= threshold_mean) {
					for (int c = s - (sd + ed); c < s - ed; c++) {
						variance_z1_q = variance_z1_q + pow(abs(vecdata[c].z1 - mean_z1_q), 2);
						variance_z2_q = variance_z2_q + pow(abs(vecdata[c].z2 - mean_z2_q), 2);
					}
					for (int c = s + ed; c < s + ed + sd; c++) {
						variance_z1_h = variance_z1_h + pow(abs(vecdata[c].z1 - mean_z1_h), 2);
						variance_z2_h = variance_z2_h + pow(abs(vecdata[c].z2 - mean_z2_h), 2);
					}
					if (variance_z1_q < threshold_variance&&variance_z2_q < threshold_variance &&variance_z1_h < threshold_variance&&variance_z2_h < threshold_variance) {
#ifdef DEBUG
						printf("variance_z1_q=%f,variance_z1_h=%f,variance_z2_q=%f,variance_z2_h=%f\n", 
							variance_z1_q, variance_z1_h, variance_z2_q, variance_z2_h);
						printf("sss==%d\n", s);
#endif // DEBUG
						//添加s至resultpoint
						dp.loc_num = resultpoint.size() + 1;
						dp.type = CONNECTOR_JOINT;
						dp.latitude = vecdata[s].lat;
						dp.longitude = vecdata[s].lon;
						dp.stand_value = 0.2;
						dp.measure_value = (abs(mean_z1_q - mean_z1_h) + abs(mean_z2_q - mean_z2_h))*0.5;
						dp.qualified = false;
						resultpoint.push_back(dp);

					}
					s = s + 199;
				}

			}
			i++;
		}


	}
	return 0;
}
int sdCalculate(vector<DetectPoint>& Result_Points,
	float conn_stand, 
	float expand_stand, 
	float camera_accuracy) {

	if (datalist.size() < 1) {
		printf("data size error!\n");
		return LOAD_DATA_ERROR;
	}

	vector<fun_point> ex_points;//存放扫描排除点
	vector<fun_point> fun_points;//存放兴趣点
	int err = findFunPoint(datalist, fun_points);
	if (err != SUCCESS) {
		return err;
	}

	int res = handleFunPoint(fun_points);
	if (res != SUCCESS) {
		return res;
	}

	judgmentItem(datalist, fun_points,ex_points, Result_Points);


	int result = scanOddPoint(datalist, ex_points, Result_Points);


}

vector<CellData> cutData(vector<CellData> original_data) {
	//count(original_data);
	vector<float> z1;
	vector<float> z2;
	float mid1;
	float mid2;
	bool needCutStart = false;
	bool needCutEnd = false;
	double startCore = 0;

	int endCore = original_data.size();
	for (vector<CellData>::iterator it = original_data.begin(); it != original_data.end(); it++) {
		z1.push_back(it->z1);
		z2.push_back(it->z2);
	}

	//先判断是否需要掐头去尾
	if (count(z1.begin(), z1.begin() + 500, 0.0) > 100 && count(z2.begin(), z2.begin() + 500, 0.0) > 100) {
		needCutStart = true;
	}
	if (count(z1.end()-500, z1.end(), 0.0) > 100 && count(z2.end()-500, z2.end() , 0.0) > 100) {
		needCutEnd = true;
	}
	//计算中位数mid1、mid2
	sort(z1.begin(), z1.end());
	sort(z2.begin(), z2.end());
#ifdef DEBUG
	printf("z1 mid==%f,z2 mid==%f\n", z1[z1.size() / 2], z2[z2.size() / 2]);
#endif
	if ((z1[z1.size() / 2] - z2[z2.size() / 2]) <= 5) {
		mid1 = z1[z1.size() / 2];
		mid2 = z2[z2.size() / 2];
	}
	else {
		//return error;
	}
	//掐头 

	if (needCutStart) {
#ifdef DEBUG
		printf("Need cut start!!\n");
#endif
		int b = 0;
		int core = -1;
		//利用mid计算出dt，标记dt小于波动系数的开始坐标
		for (vector<CellData>::iterator it = original_data.begin(); it != original_data.end(); it++) {

			if (abs(it->z1 - mid1) > 5|| abs(it->z2 - mid2) > 5){
				b = 0;
				core = -1;
			}
			else {
				//printf("b=b+1=%d", b);
				b = b + 1;
				if (b == 1) {
					core = it->num;
				}

			}
			
			if (b == 1000) {
#ifdef DEBUG
				printf("startb=%d\n", core);
#endif
				startCore = core;
				break;
			}
			
		}
	}
	//去尾
	if (needCutEnd) {
#ifdef DEBUG
		printf("Need cut End!!\n");
#endif
		int b = 0;
		int core = -1;
		//利用mid计算出dt，标记dt小于波动系数(默认为5)的结束坐标
		for (vector<CellData>::reverse_iterator it = original_data.rbegin(); it != original_data.rend(); it++) {

			if (abs(it->z1 - mid1) > 5 || abs(it->z2 - mid2) > 5) {
				b = 0;
				core = -1;
			}
			else {
				//printf("b=b+1=%d", b);
				b = b + 1;
				if (b == 1) {
					core = it->num;
				}

			}

			if (b == 1000) {
#ifdef DEBUG
				printf("endb=%d\n", core);
#endif
				endCore = core;
				break;
			}

		}
		//for_each(original_data.begin(), original_data.begin() + original_data.size() / 2, ForDValue(mid));
	}
	vector<CellData> cut_data;
	cut_data.resize(endCore - startCore);
	//(*cutResult.resize(endCore - startCore);
	copy(original_data.begin()+ startCore, original_data.begin()+endCore, cut_data.begin());
	for (int i = 0; i < cut_data.size(); i++) {
		cut_data[i].num = i;
	}
	return cut_data;
}