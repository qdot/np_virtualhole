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
	np_virtualhole() :
	m_changeCount(0)
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
	virtualhole_info m_virtualInfo[10];
	int m_speedValue[9];
	int m_changeCount;
	

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
			virtualhole_set_speeds(&m_virtualHole, m_virtualInfo, m_changeCount);
			m_changeCount = 0;				
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
		m_changeCount = 0;				
		for(int i = 0; i < 9; ++i)
		{
			if(m_speedValue[i] != GetInt(argv[i]))
			{
				m_speedValue[i] = GetInt(argv[i]);
				m_virtualInfo[m_changeCount].motor = i;
				m_virtualInfo[m_changeCount].speed = GetInt(argv[i]);
				++m_changeCount;
			}
		}
	}

private:
	FLEXT_CALLBACK_A(virtualhole_anything)
	FLEXT_CALLBACK_V(set_speed)
};

FLEXT_NEW("np_virtualhole", np_virtualhole)



