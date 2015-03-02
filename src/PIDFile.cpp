
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <fcntl.h>

#include <libclientserver.h>

PIDFile::PIDFile(std::string Filename)
{
	m_IsOwner = false;
	m_filename = Filename;
}

PIDFile::~PIDFile()
{
	if (IsOwner())
	{
		Remove();
	}
}

/**
 * Create
 *
 * Create a new PIDFile using the filename that was specified in the constructor.
 * The function will return true if it was capable of creating a pid file.
 * The function will fail if the pid file already exists and has an active owner
 * An active owner is checked by comaring the current process exe filename with
 * the process who's pid is in the existing pid file
 */
bool PIDFile::Create()
{
	int fd = open(m_filename.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP);
	FILE *file = NULL;
	int tpid = -1;
	int ret = 0;

	if (IsOwner())
		abort();	//Error with caller attempting to create pidfile twice
	
	if (fd < 0)
	{
		switch(errno)
		{
			case EEXIST: //Lets see if we can fix fd
				file = fopen(m_filename.c_str(), "r");
				if (!file)
					return false;
				
				ret = fscanf(file, "%d", &tpid);
				fclose(file);
				
				if (ret != 1) //Issue here is that we have an unparsable pid file. Should we remove it?
					return false;

				do {
					std::string self;
					std::string pidexe;
					
					if (GetOwnExePath(&self) == false)
						return false;
					
					if (GetPIDExePath(tpid, &pidexe) == true)
					{
						if (self == pidexe)
							return false; //The pid in the file matchs us and is a valid process
					}
					
					if (unlink(m_filename.c_str()) < 0) //Not "us" just overwrite it
							abort();
					
					fd = open(m_filename.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP);
					if (fd < 0)
						return false;
				
				} while(0);

				break;
			default:
				return false;
		}
	}

	FILE *fp = fdopen(fd, "w");
	if (!fp)
		abort();	//Should never happen
	fprintf(fp, "%d\n", getpid());
	fclose(fp);
	m_IsOwner = true;
	return true;
}

/**
 * Remove
 *
 * After calling Create this function can be used to remove the pid file
 * If the PIDFile is invalid and is not currently owner by the process
 * calling Remove will call abort.
 */
void PIDFile::Remove()
{
	if (IsOwner() == false)
		abort(); //Your not the owner

	if (unlink(m_filename.c_str()) < 0)
		abort(); //We should be the owner of this file

	m_IsOwner = false;
}

/**
 * IsOwner
 *
 * This function can be used to determin if it is safe to call the Remove function
 * The return value will be true if a valid PIDFIle was created by the object but not yet removed
 */
bool PIDFile::IsOwner()
{
	return m_IsOwner;
}

bool PIDFile::GetOwnExePath(std::string *name)
{
	char buf[256];
	
	if (readlink("/proc/self/exe", buf, sizeof(buf)) < 0)
		return false;
	*name = buf;
	return true;
}

bool PIDFile::GetPIDExePath(int pid, std::string *name)
{
	char buf1[256];
	char buf2[256];
	
	sprintf(buf1, "/proc/%d/exe", pid);
	int ret = readlink(buf1, buf2, sizeof(buf2));
	if (ret < 0)
		return false;
	buf2[ret] = 0;
	*name = buf2;
	return true;
}



