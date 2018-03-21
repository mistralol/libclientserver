
#include <libclientserver.h>

#include <openssl/rand.h>
#include <openssl/evp.h>

std::string Crypto::SHA1Pass(const std::string pass, const std::string salt)
{
	std::stringstream ss;
	unsigned char out[20];

	int ret = PKCS5_PBKDF2_HMAC_SHA1(pass.c_str(), 0, (unsigned char *) salt.c_str(), salt.size(), 1000, sizeof(out), out);
	if (ret != 1)
		abort();

	for(size_t i=0;i<sizeof(out);i++)
	{
		char tmp[3];
		sprintf(tmp, "%02X", out[i] & 0xFF);
		ss << tmp;
	}
	return ss.str();
}

