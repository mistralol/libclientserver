
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


