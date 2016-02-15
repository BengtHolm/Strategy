#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>	// invalid_argument
using namespace std;

//--------------------------------------
//    Strategy CLASS Declaration
//--------------------------------------

class TaksatorStrategy
{
public:
	enum ETaksator { HyggeSnak, Normal, Web };
	virtual int  Takser( int min ) = 0;
	virtual string StrategyType() = 0;
};

class HyggeSnakTaksering : public TaksatorStrategy
{
public:
	int Takser( int min );
	string StrategyType() { return "HyggeSnak"; };
};

class NormalTaksering : public TaksatorStrategy
{
public:
	int Takser( int min );
	string StrategyType()  { return "Normal"; };
};

class WebTaksering : public TaksatorStrategy
{
public:
	int Takser( int min );
	string StrategyType() { return "Web"; };
};

TaksatorStrategy* CreateTaksator( TaksatorStrategy::ETaksator t )
{
	switch( t )
	{
	case TaksatorStrategy::HyggeSnak:
		return new HyggeSnakTaksering;

	case TaksatorStrategy::Normal:
		return new NormalTaksering;

	case TaksatorStrategy::Web:
		return new WebTaksering;
	}
	throw invalid_argument( "invalid argument t in CreateTaksator" );
}


//--------------------------------------
//    Strategy CLASS Implementation
//--------------------------------------

int HyggeSnakTaksering::Takser( int min )
{
	const int sats1 = 25;
	const int sats2 = 10;

	int amt = min * sats1;

	if( min > 30 )
		amt -= (min - 30) * sats2;

	return amt;
}


int NormalTaksering::Takser( int min )
{
	const int sats = 22;

	return min * sats;
}


int WebTaksering::Takser( int min )
{
	const int low_sats  = 8;
	const int high_sats = 22;

	return (min > 60) ? min * low_sats : min * high_sats;
}



//------------------------------------------
//    Test CLASS Declaration
//    ("TOT" står for telegram og taksering)
//    (- ikke noget med død)
//
//    Bemærk: Dette er en minimal løsning
//    Udvidelser kunne være
//    - Parameteriserede strategiklasser
//    - overfør container til strategiklasse
//------------------------------------------

class TotApp
{
public:
	TotApp()	{ m_pTaksator = 0; }
	~TotApp()	{ delete m_pTaksator; }

	void SetTaksStrategy( TaksatorStrategy::ETaksator t );

	void ComputeList();
	void LoadList();
	void WriteList();

private:
	vector<int> a;
	TaksatorStrategy* m_pTaksator;
};


void TotApp::SetTaksStrategy( TaksatorStrategy::ETaksator t )
{
	delete m_pTaksator;
	m_pTaksator = CreateTaksator( t );
}

void TotApp::ComputeList()
{
	if( m_pTaksator )
	{
		int total = 0;
		for( vector<int>::size_type i = 0; i < a.size(); i++ )
			total += m_pTaksator->Takser( a[i] );

		cout << "Med " << m_pTaksator->StrategyType()
			 << " strategi koster det " << total/100.0 << " kr." << endl;
	}
	else
		cout << "Der er ikke valgt strategi ......\n";
}

void TotApp::LoadList()
{
	a.push_back( 21 );
	a.push_back( 74 );
	a.push_back( 38 );
	a.push_back( 15 );
	a.push_back( 65 );
}


void TotApp::WriteList()
{
	for( vector<int>::size_type i = 0; i < a.size(); i++ )
		cout << i << ": " << a[i] << endl;
}



//--------------------------------------
//    Test Application
//--------------------------------------

int main()
{
	TotApp ta;

	ta.LoadList();
	ta.WriteList();
	ta.ComputeList();
	cout << "-----------------\n";

	ta.SetTaksStrategy( TaksatorStrategy::HyggeSnak );
	ta.ComputeList();
	cout << "-----------------\n";

	ta.SetTaksStrategy( TaksatorStrategy::Normal );
	ta.ComputeList();

	cout << "-----------------\n";
	ta.SetTaksStrategy( TaksatorStrategy::Web );
	ta.ComputeList();
	cout << endl;

	return 0;
}
