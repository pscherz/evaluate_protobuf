#include <iostream>
#include <fstream>

#include <message.pb.h>

std::string generate_name()
{
	char buf[17] = {};

	size_t len = ((unsigned int)std::rand() & 15) + 1;

	buf[0] = (char)((std::rand()%25) + 'A');
	for (size_t i = 1, end = (size_t)len; i < end; ++i)
	{
		buf[i] = (char)((std::rand() % 25) + 'a');
	}

	return buf;
}

void write()
{
	// Frustrationslevel: Medium.
	// Binary wird riesig, wenn statisch eingebunden (+1MB mindestens).
	// Viel Code läuft vor main().
	// Auf Linux geht statisch einbinden seit 3.6.x nicht mehr, stürzt immer vor main() ab.
	// Auf Windows scheint dynamisch einbinden nicht zu gehen, wegen Kompilierfehlern bezüglich DLL-Export Verschiedenheiten mit den generierten Klassen.
	// Kann auch als mutierbarer Buffer verwendet werden.

	Message::Addressbook people;

	for (size_t i = 0; i < 1000; ++i)
	{
		Message::Person *person = people.add_people();
		person->set_name(std::string(generate_name()));
		person->set_id(i);
	}

	std::ofstream out;
	out.open("out.bin", std::ios::binary);

	people.SerializeToOstream(&out);
	out.close();
}

std::string read(const uint8_t *data, int data_len)
{
	Message::Addressbook people;
	people.ParseFromArray(data, data_len);

	std::string retval;
	retval.reserve(64);
	retval += people.people().begin()->name() + " ";
	retval += people.people().rbegin()->name();

	return retval;
}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int main()
{
	std::srand(222);
	LARGE_INTEGER freq, start, stop;

	write();

	std::ifstream in;
	in.open("out.bin", std::ios::binary);

	in.seekg(0, std::ios::end);
	size_t data_len = in.tellg();
	in.seekg(0, std::ios::beg);

	uint8_t *data = new uint8_t[data_len];
	in.read((char*)data, data_len);
	in.close();

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	for (size_t i = 0; i < 1000; ++i)
	{
		if (read(data,(int)data_len)[0] == 'a') { // just so the compiler won't optimize away
			std::cout << 'a' << std::endl;
		}
	}

	QueryPerformanceCounter(&stop);
	stop.QuadPart -= start.QuadPart;

	std::cout << "Execution took " << ((stop.QuadPart * 1000.f) / freq.QuadPart) << "ms";

	std::cin.ignore(1024, '\n');
	return 0;
}