
/**
 * @class PIDFile
 * @brief Used for managment of a PID file
 *
 * This class is capable of managing a PID file on the fileystem
 * so the external processes can find our PID in an easy manner.
 * Or to prevent a process from starting multiple instances of its self.
 */

class PIDFile
{
	public:
		PIDFile(std::string Filename);
		~PIDFile();
		
		bool Create();
		void Remove();
		bool IsOwner();
		
	private:
		bool GetOwnExePath(std::string *name);
		bool GetPIDExePath(int pid, std::string *name);
		
	private:
		std::string m_filename;
		bool m_IsOwner;

};


