// include flext header
#include <flext.h>
#include <virtualhole.h>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


class np_virtualhole:
	// inherit from basic flext class
	public flext_base
{
	// obligatory flext header (class name,base class name)
	FLEXT_HEADER(np_virtualhole,flext_base)
 
public:
	// constructor
	np_virtualhole()
	{
		virtualhole_init(&m_virtualHole);
		AddInAnything("Command Input");		
		AddInList("Speed Input (List of 9 ints)");
		AddOutBang("Bangs on successful connection/command");
		
		FLEXT_ADDMETHOD(0, virtualhole_anything);
		FLEXT_ADDMETHOD(1, set_speed);
	} 

	virtual ~np_virtualhole()
	{
		virtualhole_close(&m_virtualHole);
	}
	
protected:
	virtualhole_device m_virtualHole;
	int m_speedValue[9];

	void virtualhole_anything(const t_symbol *msg,int argc,t_atom *argv)
	{
		
		
		if(!strcmp(msg->s_name, "open"))
		{
			int ret;
			
			if(argc == 1)
			{
				post("Opening %d", GetInt(argv[0]));
				ret = virtualhole_open(&m_virtualHole, GetInt(argv[0]));
			}
			else
			{
				post("Opening default");
				ret = virtualhole_open(&m_virtualHole, 0);
			}
			if(ret >= 0)
			{
				ToOutBang(0);
			}
			else
			{
				post("Cannot connect to virtualhole");
			}
		}
		else if (!strcmp(msg->s_name, "count"))
		{
			post("Virtual Holes Connected to System: %d", virtualhole_get_count(&m_virtualHole));
			ToOutBang(0);
		}
		else if (!strcmp(msg->s_name, "close"))
		{
			close();
			ToOutBang(0);
		}
		else if (!strcmp(msg->s_name, "bang"))
		{			
			for(int i = 0; i < 9; ++i)
			{
				virtualhole_set_speed(&m_virtualHole, i, m_speedValue[i]);
			}
			ToOutBang(0);
		}
		else 
		{
			post("Not a valid np_virtualhole message: %s", msg->s_name);
		}
	}

	void close()
	{
		virtualhole_close(&m_virtualHole);
	}

	void set_speed(int argc, t_atom* argv)
	{
		if(argc != 9)
		{
			post("speed requires a list of 9 integers");
			return;
		}
		m_speedValue[0] = GetInt(argv[0]);
		m_speedValue[1] = GetInt(argv[1]);
		m_speedValue[2] = GetInt(argv[2]);
		m_speedValue[3] = GetInt(argv[3]);
		m_speedValue[4] = GetInt(argv[4]);
		m_speedValue[5] = GetInt(argv[5]);
		m_speedValue[6] = GetInt(argv[6]);
		m_speedValue[7] = GetInt(argv[7]);
		m_speedValue[8] = GetInt(argv[8]);
	}

private:
	FLEXT_CALLBACK_A(virtualhole_anything)
	FLEXT_CALLBACK_V(set_speed)
};

FLEXT_NEW("np_virtualhole", np_virtualhole)



