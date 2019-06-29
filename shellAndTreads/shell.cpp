#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;
void functioncall();
int main()
{

	long compTime;
	struct timeval start;
	struct timeval finish;
	double Time[20], Time_Sys[20], Time_pipe[20], prom, prom2, prom3;

	for (int i = 0; i <= 20; i++)
	{
		gettimeofday(&start, 0);
		functioncall();
		gettimeofday(&finish, 0);
		compTime = (finish.tv_sec - start.tv_sec) * 1000000;
		compTime = compTime + (finish.tv_usec - start.tv_usec);
		Time[i] = compTime / 1000000.0;

		char syscall[20] = " ";
		gettimeofday(&start, 0);
		write(1, syscall, 0);
		gettimeofday(&finish, 0);
		compTime = (finish.tv_sec - start.tv_sec) * 1000000;
		compTime = compTime + (finish.tv_usec - start.tv_usec);
		Time_Sys[i] = compTime / 1000000.0;

		int pipe1[2];
		int pipe2[2];
		char fixed[] = "1";
		pid_t p;

		if (pipe(pipe1) == -1 && pipe(pipe2) == -1)
		{
			cout << "Failed " << endl;
			return 1;
		}
		gettimeofday(&start, 0);
		p = fork();
		if (p < 0)
		{
			cout << "Fork Failed" << endl;
			return 1;
		}
		else if (p > 0)
		{
			char concat_str[100];
			close(pipe1[0]);
			close(pipe1[1]);
			wait(NULL);
			close(pipe2[1]);
			read(pipe2[0], concat_str, 100);
			close(pipe2[0]);
		}
		else
		{
			close(pipe1[1]);
			char concat_str[100];
			read(pipe1[0], concat_str, 100);
			int k = strlen(concat_str);
			for (int i = 0; i < strlen(fixed); i++)
				concat_str[k++] = fixed[i];

			concat_str[k] = '\0';
			close(pipe1[0]);
			close(pipe2[0]);
			close(pipe2[1]);
			exit(0);
		}
		gettimeofday(&finish, 0);

		compTime = (finish.tv_sec - start.tv_sec) * 1000000;
		compTime = compTime + (finish.tv_usec - start.tv_usec);
		Time_pipe[i] = compTime / 1000000.0;
	}
	for (int j = 0; j <= 20; j++)
	{
		j == 20 ? prom = prom / 20.0, prom2 = prom2 / 20.0, prom3 = prom3 / 20.0 : prom += Time[j], prom2 += Time_Sys[j], prom3 += Time_pipe[j];
	}
	cout << "Promedio de tiempo al ejecutar una funcion: " << prom << endl;
	cout << "Promedio de tiempo al ejecutar un Syscall: " << prom2 << endl;
	cout << "Promedio de tiempo al ejecutar un Pipes: " << prom3 << endl;

	return 0;
}
void functioncall()
{
	cout << "";
}
