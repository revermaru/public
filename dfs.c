#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <setjmp.h>

#define _Event_		( sizeof (struct inotify_event) )	/* Inotify 함수의 이벤트 처리를 위한 전처리 지정 */
#define Buffer_size	( 1024 * ( _Event_ + 64 ) )

jmp_buf point;	/* setjmp 모듈을 활용할 변수 지정 */

int main( int argc, char **argv ) {
	setjmp(point);	/* setjmp 함수 당 포인트를 셋팅 한다. */
	FILE *Write_file;	/* 로그 파일을 기록하기 위한 변수 선언 */

	char Fs_Path[4092]; /* 감시할 파일 경로를 입력받을 변수 */
	strcpy(Fs_Path, argv[1]);
	
	int ii;		/* 각 버퍼 사이즈 비교 대입 값을 위한 변수 */
	
	int Inotify_Buff; /* inotify 함수를 대입할 고정 버퍼값을 위한 변수 */
	
	char buffer[Buffer_size];	
	
	ii = inotify_init();	/* inotify 모듈을 초기화 로드 한다. */
	
int Back_Target() {
		char cpcon[2048] = "cp -rf ", bakpath[] = " /tmp/bak_path"; 	/* 파일 백업을 만들 변수 */
		system("rm -rf /tmp/bak_path");
		strcat(cpcon, Fs_Path);
		strcat(cpcon, bakpath);
		system(cpcon); /* 타겟 파일 백업을 만든다. */
	}
int syscon() {		/* 이벤트 발생시 해당 파일을 복원할 파일 시스템 제어 기초 함수 설계 */ 
		
		/* 시작 기존의 백업 파일을 삭제 */
		
		char Remove_Target[2048] = ("rm -rf ");		
		strcat(Remove_Target, Fs_Path);
		system(Remove_Target);
		
		/* 완료 기존의 백업 파일을 삭제 */
		
		/* 시작 변동시 복원을 위한 작업 */
		
		char Restore[2048] = "cp -rf /tmp/bak_path ";
		strcat(Restore, Fs_Path);
		system(Restore);
		
		/* 완료 변동시 복원을 위한 작업 */
		
		/* 시작 작업 내용 출력 및 시작 로그 기록 */ 
		
		Write_file = fopen("dfs.log", "a");
		fprintf(stderr,"\nTarget ' %s ' has restored.\n\n", Fs_Path);
		fprintf(Write_file, "Target ' %s ' has restored.\n\n", argv[1]);
		fclose(Write_file);
		
		/* 완료 작업 내용 출력 및 시작 로그 기록 */
	}


	if (ii < 0) {		/* 버퍼 비교에 의한 inotify 함수 초기화 */
		perror("inotify_init");
	}
	else
	
	system("date -R >> dfs.log; echo Defending_Start >> dfs.log");	/* 날짜 출력과 함께 로그 기록을 생성하고 시작 (존재 할시 이어서 기록한다. */
	
	if (argc < 2 ) { 	/* 외부 인자 값의 유.무를 파악 하여 작업 선언 */
		printf ("Do not start defend file system... Please running with insert Target path.\n\n BYE~!\n");
		exit (0);
	} else {
	
		if ( access(Fs_Path, F_OK) == -1 ) { /* 타겟 파일의 존재를 파악하여 없다면 종료 시킨다. */
			fprintf(stderr, "Target %s does not exist... to EXIT.", Fs_Path);
			exit (0);
		} 
	
		fprintf (stderr, "Defending start... \n File path = '%s' of systems\nIf you wnat exit to 'CTRL +C'.\n", argv[1]);

		Back_Target();	/* 타깃 파일을 백업 한다. */
	}
	
	Inotify_Buff = inotify_add_watch(ii, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF);	/* inotify 각 이벤트 발생에 따른 함수 실행을 위한 변수 지정 */
	
	while(1) {	/* 지정한  타겟 파일에 대한 반복 감시 시작 */ 
		int origin_size, i = 0;
		origin_size = read(ii, buffer, Buffer_size);
		if (origin_size < 0) {
			perror("read");
		}
		
		while ( i < origin_size ) {	/* 이벤트 발생을 위한 반복 수행 */
			struct inotify_event *event = (struct inotify_event *) &buffer[i];	

			fprintf(stderr, "\nVariance check = Wd : %d , Mask : %d , Cookie : %d , Length : %d , Type : %s\n\n", event->wd, event->mask, event->cookie, event->len, (event->mask & IN_ISDIR)?"Directory":"File");	/* 타깃 파일에 대한 변화를 감지 하여 변화된 실제 파일의 타입을 출력하고 로그를 기록한다. */
			
			system("date -R >> dfs.log");
			
			Write_file = fopen("dfs.log", "a");
			
			fprintf(Write_file, "\nVariance check = Wd : %d , Mask : %d , Cookie : %d , Length : %d , Type : %s\n\n", event->wd, event->mask, event->cookie, event->len, (event->mask & IN_ISDIR)?"Directory":"File");	/* 변화된 사항과 만일 타깃 디렉터레에 추가된 사항이면 그것이 어떤 타입인지 확인 */
			
			fclose(Write_file);

			if (event->mask & IN_CREATE) {	/* 타깃 디렉터리에서 파일이나 디렉토리가 생성 되었을 경우 이벤트 처리 */
			
				printf("\n %s was created. at Watching in Target Directory.\n\n", event->name);
				
				/* 시작 출력과 출력 로그를 기록한다. */
				
				Write_file = fopen("dfs.log", "a");
				fprintf(Write_file, "\n %s was created in Target Directory. \n\n", event->name);
				fclose(Write_file);
				syscon();
				}
				
				/* 완료 출력과 출력 로그를 기록한다. */
				
			else if (event->mask & IN_DELETE || IN_DELETE_SELF) {	/* 타깃에 삭제가 발생할 경우 이벤트 처리 */

				printf(" %s was deleted at Watching Target.\n", event->name);
						
				Write_file = fopen("dfs.log", "a");
				fprintf(Write_file, " %s was deleted at Watching Target. \n", event->name);
				fclose(Write_file);
				syscon(); 
				}
				
			else if (event->mask & IN_MODIFY ) {	/* 타깃에 변동이 발생 되었을 경우 이벤트 처리 */ 

				printf(" %s was modified at Watching Target.\n", event->name);
				Write_file = fopen("dfs.log", "a");
				fprintf(Write_file, "%s was created. at Watching Target of file system.\n", event->name);
				fclose(Write_file);
				syscon(); 
				}

			else if (event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO || event->mask & IN_MOVE_SELF) {	/* 타깃에 대한 이동 발생시 이벤트 처리 */

				printf("%s was moved at Watching Target.\n", event->name);
				Write_file = fopen("dfs.log", "a");
				fprintf(Write_file, "%s was moved at Watching Target.\n", event->name);
				fclose(Write_file);
				syscon(); 
				}
			i = 0;
			longjmp(point, 1);	/* jmp 셋팅한 지점으로 다시 되돌아 간다. */
			}
		}
	return 0;
	}
