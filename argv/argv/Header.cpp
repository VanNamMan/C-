#include "Header.h"

int main(int argc , char* argv[])
{
	//cout << "Program : " << argv[0] << endl;
	try
	{
		if (argc >= 3)
		{
			cout << "Input Image : " << argv[1] << endl;
			clock_t t0 = clock();
			Mat img = imread(argv[1], 1);
			if (!img.empty())
			{
				Mat copy,gray, binary;
				copy = img.clone();
				vector<vector<Point>> contours;
				vector<vector<vector<Point>>> clusters;

				cvtColor(copy, gray, COLOR_BGR2GRAY);
				threshold(gray, binary, 155, 255, 0);
				findContours(binary, contours, RETR_EXTERNAL, 2);

				myKmean(contours, clusters);

				CString cs;
				cs.Format(_T("n clusters : %d"), clusters.size());
				print(cs);

				RNG rng(0xFFFAAA);
				for (int i = 0; i < clusters.size(); i++)
				{
					vector<vector<Point>> v = clusters[i];
					vector<Point> vs, hull;
					vstack(v, vs);
					cs.Format(_T("len clusters %d : %d"), i, vs.size());
					print(cs);

					convexHull(vs, hull);
					v = { hull };
					drawContours(copy, v, -1, randomColor(rng), 2);
				}

				t0 = clock() - t0;
				cout << "tactime : " << (float)t0 / CLOCKS_PER_SEC << endl;

				if (imwrite(argv[2], copy))
				{
					cout << "output file : "<< argv[2] << endl;
				}
				else
					cout << "No such output file for writing" << endl;

				imshow("", copy);
				waitKey(0);
			}
			else
				cout << "No such input file for reading" << endl;


		}
	}
	catch (Exception e)
	{
		return -1;
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
void myKmean(vector<vector<Point>> cnts, vector<vector<vector<Point>>> &clusters)
{
	vector<int> labels;
	int n = cnts.size();

	for (int i = 0; i < n; i++)
		labels.push_back(i);

	for (int i = 0; i < n; i++)
	{
		vector<vector<Point>> clus;
		if (labels[i] == i)
		{
			clus.push_back(cnts[i]);
			for (int j = i + 1; j < n; j++)
			{
				if (labels[j] == j)
				{
					if (isInCnts(cnts[j],clus))
					{
						clus.push_back(cnts[j]);
						labels[j] = i;
					}
				}
			}
		}
		if (clus.size() > 0)
			clusters.push_back(clus);
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

bool isNearest(vector<Point> cnt1, vector<Point> cnt2, float d)
{
	for (int i = 0 ; i < cnt1.size(); i++)
	{
		for (int j = 0; j < cnt2.size(); j++)
		{
			if (distance(cnt1[i], cnt2[j]) < d) 
				return true;
			else if (i == cnt1.size()-1 && j == cnt2.size() - 1) 
				return false;
		}
	}
}

bool isInCnts(vector<Point> cnt, vector<vector<Point>> cnts)
{
	for (int i = 0; i < cnts.size(); i++)
	{
		if (isNearest(cnt, cnts[i],50))
			return true;
		else if (i == cnts.size() - 1)
			return false;
	}
}
static Scalar randomColor(RNG& rng)
{
	int icolor = (unsigned)rng;
	return Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}
