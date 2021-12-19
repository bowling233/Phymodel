#include <glm\glm.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
#include <sstream>

#include "Object.h"
#include "Collision.h"

#include <chrono>
using namespace chrono;
ofstream ofstrm;

int main(int argc, char * argv[])
{//五个参数：默认参数、数据路径、个数、输出路径、纳秒数
	auto start = system_clock::now();
	if (argc != 5) {
		cerr << "error:arguments are incorrect" << endl;
		exit(EXIT_FAILURE);
	}
	ofstrm.open(argv[3]);
	if ((!ofstrm)) {
		cerr << "error:can't open output file" << endl;
		cerr << ofstrm.eof() << ofstrm.bad() << ofstrm.fail() << ofstrm.good() << std::endl;
		exit(EXIT_FAILURE);
	}
	ofstrm << "Computer Simulation of Rigid Sphere System" << endl << "--Physical analysis, algorithm design and visualization based on OpenGL/C++" << endl;
	ifstream ifstrm(argv[1]);
	if ((!ifstrm)) {
		cerr << "error:can't open data file" << endl;
		cerr << ifstrm.eof() << ifstrm.bad() << ifstrm.fail() << ifstrm.good() << std::endl;
		exit(EXIT_FAILURE);
	}
	int num;	stringstream ss;	ss << argv[2];	ss >> num;
	int num2;   ss << argv[4]; ss >> num2;
	ofstrm << "--------------------------Gas Infomation----------------------------------------" << endl
		<< "Process\tSystemT\tSystemTe\tCollision\t" << endl;

	CollisionSystem system(num, ifstrm);
	auto current = system_clock::now();
	ofstrm << double(duration_cast<microseconds>(current - start).count()) / microseconds::period::num / microseconds::period::den << '\t'
		<< system.time() << '\t' << system.temperature() << '\t' << endl;

	//Main circle
	for(int i = 0;i!=num2;i++)
	{
		system.run(1e-9);//nano second
		current = system_clock::now();
		ofstrm << double(duration_cast<microseconds>(current - start).count()) / microseconds::period::num / microseconds::period::den << '\t'
			<< system.time() << '\t' << system.temperature() << '\t' << endl;
	}
	ofstrm << setprecision(8);
	ofstrm << system.time() << '\t' << system.temperature() << '\t' << endl;

	ofstrm << "success:process closed" << endl;

	exit(EXIT_SUCCESS);
}