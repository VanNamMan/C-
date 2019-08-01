#include "Header.h"

template <class T> T max_(vector<T> v)
{
	T m = v[0];
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] > m) m = v[i];
	}
	return m;
}

template <class T> T min_(vector<T> v)
{
	T m = v[0];
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] < m) m = v[i];
	}
	return m;
}
float rounded(float x , int d)
{
	return (int)(x*pow(10, d)) / (float)pow(10, d);
}
int main(int argc , char* argv[])
{
	//cout << rounded(10.0 / 3, 3) << endl;
		
		if (argc>3)
		{
			cout << "Input Image : " << argv[1] << endl;
			clock_t t0 = clock();
			Mat img = imread(argv[1], 1);
			if (!img.empty())
			{
				Mat copy,gray, binary;
				copy = img.clone();
				vector<vector<Point>> contours;
				vector<vector<Point>> clusters;

				cvtColor(copy, gray, COLOR_BGR2GRAY);
				threshold(gray, binary, 155, 255, 0);
				Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
				dilate(binary, binary, element);
				findContours(binary, contours, RETR_EXTERNAL, 2);

				int d = stoi(argv[3]);
				myKmean(contours, clusters,d);

				CString cs;
				cs.Format(_T("n clusters : %d"), clusters.size());
				print(cs);

				RNG rng(0xFFFAAA);
				for (int i = 0; i < clusters.size(); i++)
				{
					vector<Point> v = clusters[i];
					vector<Point> hull;
					
					Rect r = boundingRect(v);
					Scalar color = randomColor(rng);

					rectangle(copy, r,color, 2);
					
					convexHull(v,hull);
					if (hull.size() > 0)
					{
						vector<vector<Point>> vs = { hull };
						drawContours(copy, vs, 0,color, 2);
					}
					double s = contourArea(hull) + 0.1;
					double p = arcLength(hull, true);
					cs.Format(_T("len clusters %d : %d , s : %.2f, p : %.2f , p/s = %.2f"), i, v.size(),s,p,float(p/s));
					print(cs);
					
					float ps = rounded(float(p / s), 3);
					cout << ps << endl;
					cs.Format(_T("%d , p/s:%.2f"), i, p / s);
					CT2CA psz(cs);
		
					putText(copy,psz.m_psz,Point(r.x, r.y), CV_FONT_HERSHEY_COMPLEX, 2, color, 2);
					
				}

				t0 = clock() - t0;
				cout << "tactime : " << (float)t0 / CLOCKS_PER_SEC << endl;

				if (imwrite(argv[2], copy))
				{
					cout << "output file : "<< argv[2] << endl;
				}
				else
					cout << "No such output file for writing" << endl;

				namedWindow("Hello Kmean",CV_WINDOW_FREERATIO);
				imshow("Hello Kmean", copy);
				waitKey(0);
			}
			else
				cout << "No such input file for reading" << endl;


		}
	
	system("pause");
	return 0;
}
void print(CString str)
{
	//cout<< "aaaa";
	#ifdef UNICODE
		std::wcout << (LPCTSTR)str << endl;
	#else
		std::cout << (LPCTSTR)str << endl;
	#endif
}
void myKmean(vector<vector<Point>> cnts, vector<vector<Point>> &clusters ,int d)
{
	vector<int> labels;
	vector<Rect> vRect;
	int n = cnts.size();

	for (int i = 0; i < n; i++)
		labels.push_back(i);

	for (int i = 0; i < n; i++)
	{
		vRect.push_back(boundingRect(cnts[i]));
	}

	for (int i = 0; i < n; i++)
	{
		vector<vector<Point>> clus;
		vector<Rect> vRectClus;
		vector<Point> vPoint;
		if (labels[i] == i)
		{
			clus.push_back(cnts[i]);
			vRectClus.push_back(vRect[i]);

			for (int j = i + 1; j < n; j++)
			{
				if (labels[j] == j)
				{
					int dis = distance(vRect[j], vRectClus);
					if (dis < d)
					{
						clus.push_back(cnts[j]);
						vRectClus.push_back(vRect[j]);
						labels[j] = i;
					}
				}
			}
		}

		if (clus.size() > 0)
		{
			vstack(clus, vPoint);
			clusters.push_back(vPoint);
		}
	}

}

void vstack(vector<vector<Point>> cnts, vector<Point>& v)
{
	for (int j = 0; j < cnts.size(); j++)
		v.insert(v.begin(), cnts[j].begin(), cnts[j].end());
}

float distance(Point p1, Point p2)
{
	Point p = p2 - p1;
	return sqrt(pow(p.x,2)+ pow(p.y, 2));
}

int distance(Rect r1, Rect r2)
{
	/*Point I1(r1.x + r1.width / 2,r1.y + r1.height / 2);
	Point I2(r2.x + r2.width / 2, r2.y + r2.height / 2);*/

	vector<int> vDisX;
	vDisX.push_back(abs(r1.x - r2.x));
	vDisX.push_back(abs(r1.x - r2.x - r2.width));
	vDisX.push_back(abs(r1.x + r1.width - r2.x - r2.width));
	vDisX.push_back(abs(r1.x + r1.width - r2.x));
	
	vector<int> vDisY;
	vDisY.push_back(abs(r1.y - r2.y));
	vDisY.push_back(abs(r1.y - r2.y - r2.height));
	vDisY.push_back(abs(r1.y + r1.height - r2.y - r2.height));
	vDisY.push_back(abs(r1.y + r1.height - r2.y));

	int dis = (min_(vDisX) + min_(vDisY))/2;

	return dis;
}

int distance(Rect r, vector<Rect> vRect)
{
	vector<int> vf;
	for (int j = 0; j < vRect.size(); j++) vf.push_back(distance(r, vRect[j]));

	return min_(vf);

}
static Scalar randomColor(RNG& rng)
{
	int icolor = (unsigned)rng;
	return Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}





