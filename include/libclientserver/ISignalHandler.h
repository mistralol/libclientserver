
class ISignalHandler
{
	public:
		virtual ~ISignalHandler() { }

		virtual void SigAlarm(const siginfo_t *info) { }
		virtual void SigChild(const siginfo_t *info) { }
		virtual void SigHUP(const siginfo_t *info) { }
		virtual void SigTerm(const siginfo_t *info) { }
		virtual void SigUser1(const siginfo_t *info) { }
		virtual void SigUser2(const siginfo_t *info) { }
		virtual void SigPipe(const siginfo_t *info) { }

};


