# include <iostream> //  // cout, endl
# include <string> //  string, find_last_of, substr
# include <vector> //  vector, push_back
# include <cstdlib> //  system, atoi
# include <stdio.h>
# include <stdlib.h>

using namespace std;

int gRow;

struct Ttk{
  string type;
  int col;
  int row;
  string name;
  float val;
  bool isft;
};

struct V{
  string name;
  bool g;
  string type;
  string fname;
  string arr;
};

struct F{
  string name;
  string type;
  vector<Ttk> context;
};

vector<F> gA_Function;
vector<V> gA_Var;
F gfunction;
V gdec;
vector<string> gans;

bool gerr3;

class Scanner{
public:
  Ttk Init() {
    Ttk init;
    init.col = 0;
    init.isft = false;
    init.name = "";
    init.row = 0;
    init.type = "";
    init.val = 0;

    return init;
  } // Init()

  bool HasDot( string name ) {
    for ( int i = 0 ; i < name.size() ; i++ ) {
      if ( name[i] == '.' ) {
        return true;
      } // if    
    } // for

    return false;
  } // HasDot()

  char GetFirstChar( string input, bool & skipped, int & ip ) {
    for ( ; ip < input.size() && ( input[ip] == '\t' || input[ip] == ' ' ||
                                   input[ip] == '\n' || input[ip] == '\0' ) ; ip++ ) {  
    } // for
  
    if ( input[ip] == '/' && input[ip+1] == '/' ) {
      skipped = true;
      ip = input.size()-1;
    } // if

    if ( skipped ) {
      return '\0';
    } // if
    else {
      return input[ip]; 
    } // else

  } // GetFirstChar()

  vector<Ttk> GetTk( string curLine ) {
    char curch = '\0';
    bool skipped = false;
    Ttk curTk = Init(); // init type err
    vector<Ttk> tkStr;
    string tmp = "'";
    for ( int ip = 0 ; ip < curLine.size() ; ip++ ) {
      skipped = false;
      curch = GetFirstChar( curLine, skipped, ip ) ;
      if ( ip != curLine.size() && !skipped ) {
        curTk.name = curch;
        curTk.row = gRow;
        curTk.col = ip+1;
        if ( curch == '/' ) { // first char ='/' /,/=
          if (  ip+1 < curLine.size() && curLine[ip+1] == '=' ) {
            curTk.name = "/=";
            ip++;
            curTk.type = "de";
          } // if 
          else {
            curTk.name = "/";
            curTk.type = "/";
          } // else      
        } // if
        else if ( ( curch >= 'A' && curch <= 'Z' ) || ( curch >= 'a' && curch <= 'z' ) ) { // id
          // int,float,char,bool,string,void,if,else,while,do,return,(true,false=>constant)
          while ( ip+1 < curLine.size() 
                  && ( ( curLine[ip+1] >= 'A' && curLine[ip+1] <= 'Z' ) ||
                       ( curLine[ip+1] >= 'a' && curLine[ip+1] <= 'z' ) || 
                       curLine[ip+1] == '_' 
                       || ( curLine[ip+1] >= '0' && curLine[ip+1] <= '9' ) ) ) { // peakchar
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
          } // while
  
          if ( curTk.name == "int" ) {
            curTk.type = "int";
          } // if
          else if ( curTk.name == "float" ) {
            curTk.type = "float";
          } // else if
          else if ( curTk.name == "char" ) {
            curTk.type = "char";
          } // else if
          else if ( curTk.name == "bool" ) {
            curTk.type = "bool";
          } // else if
          else if ( curTk.name == "string" ) {
            curTk.type = "string";
          } // else if
          else if ( curTk.name == "void" ) {
            curTk.type = "void";
          } // else if
          else if ( curTk.name == "if" ) {
            curTk.type = "if";
          } // else if
          else if ( curTk.name == "else" ) {
            curTk.type = "else";
          } // else if
          else if ( curTk.name == "while" ) {
            curTk.type = "while";
          } // else if
          else if ( curTk.name == "do" ) {
            curTk.type = "do";
          } // else if
          else if ( curTk.name == "return" ) {
            curTk.type = "return";
          } // else if
          else if ( curTk.name == "true" ) {
            curTk.type = "cst";
          } // else if
          else if ( curTk.name == "false" ) {
            curTk.type = "cst";
          } // else if
          else {
            curTk.type = "id";
          } // else
        } // else if
        else if ( ( curch >= '0' && curch <= '9' ) || curch == '.' ||
                  curch == '"' || curch == tmp[0] ) { // constant first char: 0~9,.,',"
          curTk.type = "cst";
          if ( curch >= '0' && curch <= '9' ) {
            while ( ip+1 < curLine.size() && ( ( curLine[ip+1] >= '0' && curLine[ip+1] <= '9' )
                                               || curLine[ip+1] == '.' ) ) {
              if ( ( curLine[ip+1] >= '0' && curLine[ip+1] <= '9' ) ||
                   ( curLine[ip+1] == '.' &&  !HasDot( curTk.name ) ) ) {
                curTk.name = curTk.name + curLine[ip+1];
                ip++;
              } // if
              else {
                // err1
                curTk = Init();
                curTk.type = "err1";
                curTk.name = curTk.name + curLine[ip+1];
                ip++;
                curTk.row = gRow;
                curTk.col = ip+1;
              } // else
            } // while
          } // if
          else if ( curch == '.' ) {
            while ( ip+1 < curLine.size() && curLine[ip+1] >= '0' && curLine[ip+1] <= '9' ) {
              curTk.name = curTk.name + curLine[ip+1];
              ip++; 
            } // while        
          } // else if
          else if ( curch == '"' ) {
            while ( ip+1 < curLine.size() && curLine[ip+1] != '"' ) {
              curTk.name = curTk.name + curLine[ip+1];
              ip++; 
            } // while

            // add end " 
            if ( curLine[ip+1] == '"' ) {
              curTk.name = curTk.name + curLine[ip+1];
              ip++;       
            } // if
            else {
               // err1
              curTk = Init();
              curTk.type = "err1";
              curTk.name = curLine[ip];
              curTk.row = gRow;
              curTk.col = ip+1;
            } // else   
          } // else if
          else if ( curch == tmp[0] ) {
            while ( ip+1 < curLine.size() && curLine[ip+1] != tmp[0] ) {
              curTk.name = curTk.name + curLine[ip+1];
              ip++; 
            } // while
            // add end '
            if ( curLine[ip+1] == tmp[0] ) {
              curTk.name = curTk.name + curLine[ip+1];
              ip++;       
            } // if
            else {
              // err1
              curTk = Init();
              curTk.type = "err1";
              curTk.name = curLine[ip];
              curTk.row = gRow;
              curTk.col = ip+1;
            } // else     
          } // else if
        } // else if
        else if ( curch == '(' ) {
          curTk.type = "(";
        } // else if
        else if ( curch == ')' ) {
          curTk.type = ")";
        } // else if
        else if ( curch == '[' ) {
          curTk.type = "[";
        } // else if
        else if ( curch == ']' ) {
          curTk.type = "]";
        } // else if
        else if ( curch == '{' ) {
          curTk.type = "{";
        } // else if
        else if ( curch == '}' ) {
          curTk.type = "}";
        } // else if
        else if ( curch == '+' ) { // + += ++
          if (  ip+1 < curLine.size() && ( curLine[ip+1] == '=' || curLine[ip+1] == '+' ) ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
 
            if ( curTk.name == "+=" ) {
              curTk.type = "pe";
            } // if
            else {
              curTk.type = "pp";
            } // else
          } // if 
          else {
            curTk.type = "+";
          } // else   
        } // else if
        else if ( curch == '-' ) { // - -= --
          if (  ip+1 < curLine.size() && ( curLine[ip+1] == '=' || curLine[ip+1] == '-' ) ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;

            if ( curTk.name == "-=" ) {
              curTk.type = "me";
            } // if
            else {
              curTk.type = "mm";
            } // else
          } // if 
          else {
            curTk.type = "-";
          } // else
        } // else if
        else if ( curch == '*' ) { // * *=
          if (  ip+1 < curLine.size() && curLine[ip+1] == '=' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "te";
          } // if 
          else {
            curTk.type = "*";
          } // else 
        } // else if
        else if ( curch == '%' ) { // % %=
          if (  ip+1 < curLine.size() && curLine[ip+1] == '=' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "re";
          } // if 
          else {
            curTk.type = "%";
          } // else
        } // else if
        else if ( curch == '^' ) {
          curTk.type = "^";
        } // else if
        else if ( curch == '>' ) { // >= > >>
          if (  ip+1 < curLine.size() && ( curLine[ip+1] == '=' || curLine[ip+1] == '>' ) ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;

            if ( curTk.name == ">=" ) {
              curTk.type = "ge";
            } // if
            else {
              curTk.type = "rs";
            } // else
          } // if 
          else {
            curTk.type = ">";
          } // else
        } // else if
        else if ( curch == '<' ) { // <= < <<
          if (  ip+1 < curLine.size() && ( curLine[ip+1] == '=' || curLine[ip+1] == '<' ) ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;

            if ( curTk.name == "<=" ) {
              curTk.type = "le";
            } // if
            else {
              curTk.type = "ls";
            } // else
          } // if 
          else {
            curTk.type = "<";
          } // else
        } // else if
        else if ( curch == '=' ) { // == =
          if (  ip+1 < curLine.size() && curLine[ip+1] == '=' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "eq";
          } // if 
          else {
            curTk.type = "=";
          } // else
        } // else if
        else if ( curch == '!' ) { // != !
          if (  ip+1 < curLine.size() && curLine[ip+1] == '=' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "neq";
          } // if 
          else {
            curTk.type = "!";
          } // else
        } // else if
        else if ( curch == '&' ) { // & &&
          if (  ip+1 < curLine.size() && curLine[ip+1] == '&' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "and";
          } // if 
          else {
            curTk.type = "&";
          } // else
        } // else if
        else if ( curch == '|' ) { // | ||
          if (  ip+1 < curLine.size() && curLine[ip+1] == '|' ) {
            curTk.name = curTk.name + curLine[ip+1];
            ip++;
            curTk.type = "or";
          } // if 
          else {
            curTk.type = "|";
          } // else
        } // else if
        else if ( curch == ';' ) {
          curTk.type = ";";
        } // else if
        else if ( curch == ',' ) {
          curTk.type = ",";
        } // else if
        else if ( curch == '?' ) {
          curTk.type = "?";
        } // else if
        else if ( curch == ':' ) {
          curTk.type = ":";
        } // else if
        else {
          // err1
          curTk = Init();
          curTk.type = "err1";
          curTk.name = curch;
          curTk.row = gRow;
          curTk.col = ip+1;
        } // else

        if ( curTk.type == "err1" ) {
          tkStr.push_back( curTk ) ;
          ip = curLine.size();
        } // if
        else {
          if ( HasDot( curTk.name ) ) {
            curTk.isft = true;
          } // if

          tkStr.push_back( curTk );
          curTk = Init();         
        } // else
      } // if
    } // for

    return tkStr;

  } // GetTk()
};


string ReadLine( bool & eof ) {
  char ch = '\0';
  string input = "\0";
  while ( scanf( "%c", &ch ) != EOF && ch != '\n' ) {
    input = input + ch;
  } // while

  if ( ch != '\n' ) {
    eof = true;
  } // if

  return input;
} // ReadLine()

class Parser{
public:
  int mrec;
  bool mll;
  
  bool FindSameF( string name, int & ip ) {
    for ( int i = 0; i < gA_Function.size() ; i++ ) {
      if ( name == gA_Function[i].name ) {
        ip = i;
        return true;
      } // if
    } // for

    ip = -1;
    return false;
  } // FindSameF()

  bool FindSameV( string name, int & ip ) {
    for ( int i = 0; i < gA_Var.size() ; i++ ) {
      if ( name == gA_Var[i].name && gA_Var[i].fname == gfunction.name ) {
        ip = i;
        return true;
      } // if
    } // for

    ip = -1;
    return false;
  } // FindSameV()

  Ttk Get( vector<Ttk> & tkStr ) {
    Ttk tmp;
    if ( tkStr.size() > 0 ) {
      tmp = tkStr[0];
    } // if

    tkStr.erase( tkStr.begin() + 0 );
    return tmp;
  } // Get()


  bool IsUndefined( Ttk id ) {
    int ip = 0;
    gerr3 = false;
    if ( id.name != "cin" && id.name != "cout" && !IsFive_Fname( id.name ) ) {
      if ( !FindSameF( id.name, ip ) &&
           !FindSameV( id.name, ip ) ) {
        if ( mll ) {
          cout << "> ";
        } // if

        cout << "Line " << id.row - mrec ;
        cout << " : undefined identifier : '" << id.name << "'" << endl;

        mrec = id.row;
        gerr3 = true;
        return true;
      } // if
    } // if

    return false;
  } // IsUndefined()

  bool IsFive_Fname( string name ) {
    if ( name != "Done" && name != "ListFunction" && name != "ListVariable"
         && name != "ListAllVariables" && name != "ListAllFunctions" ) {
      return false;
    } // if

    return true;
  } // IsFive_Fname()

  void SortedF( vector<F> & unsort ) {
    F t;

    for ( int i = 0; i < unsort.size() ; i++ ) {
      for ( int j = i+1; j < unsort.size() ; j++ ) {
        if ( unsort[i].name > unsort[j].name ) {
          t = unsort[j];
          unsort[j] = unsort[i];
          unsort[i] = t;
        } // if
      } // for
    } // for

  } // SortedF()

  void SortedV( vector<V> & unsort ) {
    V t;

    for ( int i = 0; i < unsort.size() ; i++ ) {
      for ( int j = i+1; j < unsort.size() ; j++ ) {
        if ( unsort[i].name > unsort[j].name ) {
          t = unsort[j];
          unsort[j] = unsort[i];
          unsort[i] = t;
        } // if 
      } // for
    } // for

  } // SortedV()


  void Print() {
    if ( mll ) {
      cout << "> ";
    } // if

    for ( int i = 0; i < gans.size() ; i ++ ) {
      cout << gans[i] << endl;
    } // for

    gans.clear();
  } // Print()

  bool Five_F( vector<Ttk> tkStr, vector<Ttk> & result ) {
    int target = 0;
    if ( gA_Function.size() > 0 ) {
      if ( tkStr[0].name == "Done" ) {
        result.clear();
        return true; 
      } // if
      else if ( tkStr[0].name == "ListAllFunctions" ) {

        if ( mll ) {
          cout << "> ";
        } // if

        SortedF( gA_Function ) ;
        for ( int i = 0 ; i < gA_Function.size() ; i++ ) {
          if ( !IsFive_Fname( gA_Function[i].name ) ) {
            cout << gA_Function[i].name << "()" << endl;
          } // if 
        } // for

        cout << "Statement executed ..." << endl;

        // if ( tkStr.size() != result.size() ) {
        mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
        // } // if

        if ( mrec == result[0].row ) {
          mrec--;
        } // if

        return true; 
      } // else if
      else if ( tkStr[0].name == "ListFunction" ) {
        string nm;
        bool out = false;
        if ( mll ) {
          cout << "> ";
        } // if

        for ( int j = 0 ; j < tkStr.size() && !out ; j++ ) {
          if ( tkStr[j].name == "(" ) {
            for ( int i = 1 ; i < tkStr[j+1].name.size()-1 ; i++ ) {
              nm = nm + tkStr[j+1].name[i] ;
              out = true;
            } // for
          } // if
        } // for

        FindSameF( nm, target ) ;
        if ( target != -1 ) {
          bool printendl = false;
          int garbage = 0;
          string space = "";
          cout << gA_Function[target].type << " " << gA_Function[target].name;
          for ( int i = 0 ; target != -1 && i < gA_Function[target].context.size() ; i++ ) {
            if ( gA_Function[target].context[i].name == "{"
                 || gA_Function[target].context[i].name == "}" 
                 || gA_Function[target].context[i].name == ";" ) {
              if ( gA_Function[target].context[i].name == "{" ) {
                space = space + "  ";
              } // if 
              else if ( gA_Function[target].context[i].name == "}" ) {
                string tmp = "";
                for ( int j = 0 ; j < space.size()-2 ; j++ ) {
                  tmp = tmp + " ";
                } // for

                space = tmp;
                cout << space;
              } // else if 

              cout << gA_Function[target].context[i].name << endl;     
              printendl = true;
            } // if
            else {
              if ( printendl ) {
                cout << space;
                printendl = false;
              } // if

              cout << gA_Function[target].context[i].name;

              if ( i != gA_Function[target].context.size()-1
                   && gA_Function[target].context[i+1].name != "["
                   && gA_Function[target].context[i+1].name != "++"
                   && gA_Function[target].context[i+1].name != "--"
                   && gA_Function[target].context[i+1].name != "(" ) {
                cout << " "; 
              } // if
              else if ( gA_Function[target].context[i].name == "if"
                        || gA_Function[target].context[i].name == "while"
                        || gA_Function[target].context[i].name == "do" ) {
                cout << " ";               
              } // else if
            } // else
          } // for

        } // if
        
        cout << "Statement executed ..." << endl;

        // if ( tkStr.size() != result.size() ) {
        mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
        // } // if

        if ( mrec == result[0].row ) {
          mrec--;
        } // if

        return true;  

      } // else if
      else if ( tkStr[0].name == "ListVariable" ) {
        string nm;
        bool out = false;
        if ( mll ) {
          cout << "> ";
        } // if

        for ( int j = 0 ; j < tkStr.size() && !out ; j++ ) {
          if ( tkStr[j].name == "(" ) {
            for ( int i = 1 ; i < tkStr[j+1].name.size()-1 ; i++ ) {
              nm = nm + tkStr[j+1].name[i] ;
              out = true;
            } // for
          } // if
        } // for 
          
        FindSameV( nm, target ) ;

        if ( target != -1 ) {
          cout << gA_Var[target].type << " " << gA_Var[target].name;
          if ( gA_Var[target].arr.size() > 0 ) {
            cout << gA_Var[target].arr << " ;" << endl;
          } // if
          else {
            cout << " ;" << endl;
          } // else
        } // if

        cout << "Statement executed ..." << endl;

        // if ( tkStr.size() != result.size() ) {
        mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
        // } // if

        if ( mrec == result[0].row ) {
          mrec--;
        } // if

        return true; 
      } // else if
      else if ( tkStr[0].name == "ListAllVariables" ) {
        if ( mll ) {
          cout << "> ";
        } // if

        SortedV( gA_Var ) ;
        for ( int i = 0; i < gA_Var.size() ; i++ ) {
          if ( gA_Var[i].g ) {
            cout << gA_Var[i].name << endl;
          } // if
        } // for

        cout << "Statement executed ..." << endl;

        // if ( tkStr.size() != result.size() ) {
        mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
        // } // if

        if ( mrec == result[0].row ) {
          mrec--;
        } // if

        return true; 
      } // else if
    } // if

    return false;
  } // Five_F()

  void Renew() {
    gfunction.name = "";
    gfunction.context.clear();
    gfunction.type = "";
    gdec.name = "";
    gdec.arr = "";
    gdec.fname = "";
    gdec.g = false;
    gdec.type = "";
  } // Renew()

  bool UserInput( vector<Ttk> & tkStr,  vector<Ttk> & result ) {
    int ip = -1;
    Renew();

    if ( Def( tkStr, result ) || ( tkStr.size() == result.size() && Satement( tkStr, result, true ) ) ) {
      if (  !Five_F( tkStr, result ) ) {      // 5 f  
        Print() ;
        // if ( tkStr.size() != result.size() ) {
        mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
        // } // if

        if ( mrec == result[0].row ) {
          mrec--;
        } // if    
      } // if

      if ( tkStr[0].row != result[0].row ) {
        mll = true;
      } // if
      else {
        mll = false;
      } // else

      tkStr = result;
      Renew();
      while ( tkStr.size() > 0 && ( Def( tkStr, result ) || ( tkStr.size() == result.size()
                                                              && Satement( tkStr, result, true ) ) ) ) {
        if (  !Five_F( tkStr, result ) ) {      // 5 f 
          Print() ;
          // if ( tkStr.size() != result.size() ) {
          mrec = tkStr[ tkStr.size() - result.size() - 1 ].row;
          // } // if

          if ( mrec == result[0].row ) {
            mrec--;
          } // if
        } // if

        if ( tkStr[0].row != result[0].row ) {
          mll = true;
        } // if
        else {
          mll = false;
        } // else

        tkStr = result;
        Renew();
      } // while

      if ( tkStr.size() > 0 ) {
        return false;
      } // if
      else {
        result.clear();
        return true;
      } // else
    } // if

    return false; 
  } // UserInput()

  bool Def( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "void" ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        Renew();
        gfunction.name = tkStr[0].name;
        gfunction.type = "void";
        Get( tkStr );
        result = tkStr;
        if ( tkStr.size() > 0 && F_def_noID( tkStr, result ) ) {
          gfunction.type = "";
          return true;
        } // if
      } // if
    } // if
    else if ( tkStr.size() > 0 && Type_specifier( tkStr, result ) ) {
      string type = tkStr[0].name;
      tkStr = result;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        Renew();
        gfunction.name = tkStr[0].name;
        gfunction.type = type;
        gdec.name = tkStr[0].name;
        gdec.type = type;
        Get( tkStr );
        result = tkStr;
        if ( tkStr.size() > 0 && F_def_or_dec( tkStr, result ) ) {
          return true;
        } // if
      } // if  
    } // else if

    return false;
  } // Def()

  bool Type_specifier( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr[0].type == "int" || tkStr[0].type == "char" || tkStr[0].type == "float" ||
         tkStr[0].type == "string" || tkStr[0].type == "bool" ) {
      Get( tkStr );
      result = tkStr;
      return true;
    } // if

    return false;
  } // Type_specifier()

  bool F_def_or_dec( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && F_def_noID( tkStr, result ) ) {
      return true;
    } // if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() ) {
      gfunction.name = "";
      gfunction.type = "";
      if ( Rest_of_dec( tkStr, result, true ) ) {
        return true;
      } // if
    } // else if

    return false;
  } // F_def_or_dec()

  bool Rest_of_dec( vector<Ttk> tkStr,  vector<Ttk> & result, bool print ) {
    vector<V> name;
    V tmp;
    string ans;
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
      gdec.arr = tkStr[0].name;
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
        gdec.arr = gdec.arr + " " + tkStr[0].name + " ";
        Get( tkStr );
        result = tkStr;
        if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
          gdec.arr = gdec.arr + tkStr[0].name;
          Get( tkStr );
          result = tkStr;
        } // if
        else {
          return false;
        } // else
      } // if
      else {
        return false;
      } // else
    } // if

    while ( tkStr.size() > 0 && tkStr[0].type == "," ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        tmp.type = gdec.type;
        tmp.name = tkStr[0].name;
        Get( tkStr );
        result = tkStr;
        tmp.arr = "";
        if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
          tmp.arr = tkStr[0].name;
          Get( tkStr );
          result = tkStr;
          if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
            tmp.arr = tmp.arr + " " + tkStr[0].name + " ";
            Get( tkStr );
            result = tkStr;
            if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
              tmp.arr = tmp.arr + tkStr[0].name;
              Get( tkStr );
              result = tkStr;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // if

        name.push_back( tmp );
      } // if
      else {
        return false;
      } // else  
    } // while

    if ( tkStr.size() > 0 && tkStr[0].type == ";" ) {
      Get( tkStr );
      result = tkStr;
      if ( print ) {
        int ip = 0;
        gdec.g = true;
        gdec.fname = "";
        if ( FindSameV( gdec.name, ip ) ) {
          ans = "New definition of " + gdec.name + " entered ...";
          gans.push_back( ans );
          gA_Var[ip] = gdec;
        } // if
        else {
          gA_Var.push_back( gdec );
          ans = "Definition of " + gdec.name + " entered ...";
          gans.push_back( ans ) ;
        } // else

        for ( int i = 0; i < name.size() ; i++ ) {
          if ( FindSameV( name[i].name, ip ) ) {
            ans = "New definition of " + name[i].name + " entered ...";
            gans.push_back( ans ) ;
            gA_Var[ip].g = true;
            gA_Var[ip].type = name[i].type;
            gA_Var[ip].fname = "";
          } // if
          else {
            name[i].g = true;
            name[i].fname = "";
            gA_Var.push_back( name[i] );
            ans = "Definition of " + name[i].name + " entered ...";
            gans.push_back( ans ) ; 
          } // else
        } // for;
      } // if
      else {
        int ip = 0;
        gdec.g = false;
        if ( FindSameV( gdec.name, ip ) ) {
          gA_Var[ip] = gdec;
        } // if
        else {
          gA_Var.push_back( gdec );
        } // else

        for ( int i = 0; i < name.size() ; i++ ) {
          if ( FindSameV( name[i].name, ip ) ) {
            name[i].g = false;
            gA_Var[ip] = name[i];
          } // if
          else {
            name[i].g = false;
            gA_Var.push_back( name[i] );
          } // else
        } // for
      } // else

      return true;
    } // if

    return false;
  } // Rest_of_dec()

  bool F_def_noID( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    string ans;
    int diff;
    result = tkStr;
    gfunction.context.clear();
    if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
      gfunction.context.push_back( tkStr[0] );
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "void" ) {
        gfunction.context.push_back( tkStr[0] );
        Get( tkStr );
        result = tkStr;
      } // if
      else if ( Formal_parlst( tkStr, result ) ) {
        diff = tkStr.size() - result.size();
        for ( int i = 0 ; i < diff ; i++ ) {
          gfunction.context.push_back( tkStr[i] );
        } // for

        tkStr = result;
      } // else if

      if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
        gfunction.context.push_back( tkStr[0] );
        Get( tkStr );
        result = tkStr;
        if ( C_statement( tkStr, result ) ) {
          diff = tkStr.size() - result.size();
          for ( int i = 0 ; i < diff ; i++ ) {
            gfunction.context.push_back( tkStr[i] );
          } // for

          int ip = 0;
          if ( FindSameF( gfunction.name, ip ) ) {
            ans = "New definition of " + gfunction.name + "() entered ...";
            gA_Function[ip] = gfunction;
            gans.push_back( ans ) ;
          } // if
          else {
            gA_Function.push_back( gfunction );
            ans = "Definition of " + gfunction.name + "() entered ...";
            gans.push_back( ans ) ;
          } // else 

          for ( int i = 0; i < gA_Var.size() ; i++ ) {
            if ( gA_Var[i].fname == gfunction.name ) {
              gA_Var.erase( gA_Var.begin() + i );
              i --;
            } // if
          } // for

          return true;
        } // if
      } // if
    } // if

    for ( int i = 0; i < gA_Var.size() ; i++ ) {
      if ( gA_Var[i].fname == gfunction.name ) {
        gA_Var.erase( gA_Var.begin() + i );
        i --;
      } // if
    } // for

    return false;
  } // F_def_noID()

  bool Formal_parlst( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    V tmp;
    result = tkStr;
    if ( Type_specifier( tkStr, result ) ) {
      tmp.type = tkStr[0].name;
      tkStr = result;
      if ( tkStr.size() > 0 && tkStr[0].type == "&" ) {
        Get( tkStr );
        result = tkStr;
      } // if

      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        tmp.name = tkStr[0].name;
        Get( tkStr );
        result = tkStr;
        if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
          tmp.arr = tkStr[0].name;
          Get( tkStr );
          result = tkStr;
          if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
            tmp.arr = tmp.arr + " " + tkStr[0].name + " ";
            Get( tkStr );
            result = tkStr;
            if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
              tmp.arr = tmp.arr + tkStr[0].name;
              Get( tkStr );
              result = tkStr;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // if

        tmp.fname = gfunction.name;
        tmp.g = false;
        gA_Var.push_back( tmp );

        tmp.arr = "";
        tmp.fname = "";
        tmp.g = false;
        tmp.name = "";
        tmp.type = "";

        while ( tkStr.size() > 0 && tkStr[0].type == "," ) {
          Get( tkStr );
          result = tkStr;
          if ( Type_specifier( tkStr, result ) ) {
            tmp.type = tkStr[0].name;
            tkStr = result;
            if ( tkStr.size() > 0 && tkStr[0].type == "&" ) {
              Get( tkStr );
              result = tkStr;
            } // if

            if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
              tmp.name = tkStr[0].name;
              Get( tkStr );
              result = tkStr;
              if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
                tmp.arr = tkStr[0].name;
                Get( tkStr );
                result = tkStr;
                if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
                  tmp.arr = tmp.arr + " " + tkStr[0].name + " ";
                  Get( tkStr );
                  result = tkStr;
                  if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
                    tmp.arr = tmp.arr + tkStr[0].name;
                    Get( tkStr );
                    result = tkStr;
                  } // if
                  else {        
                    return false;
                  } // else
                } // if
                else {
                  return false;
                } // else
              } // if 

              tmp.fname = gfunction.name;
              tmp.g = false;
              gA_Var.push_back( tmp );
              tmp.arr = "";
              tmp.fname = "";
              tmp.g = false;
              tmp.name = "";
              tmp.type = "";            
            } // if 
          } // if     
        } // while

        return true;
      } // if
    } // if

    return false;
  } // Formal_parlst()

  bool C_statement( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "{" ) {
      Get( tkStr );
      result = tkStr;
      while ( tkStr.size() > 0 && ( Dec( tkStr, result ) ||
                                    ( tkStr.size() == result.size() &&
                                      Satement( tkStr, result, false ) ) ) ) { 
        tkStr = result;
      } // while

      if ( tkStr.size() > 0 && tkStr[0].type == "}" ) {
        Get( tkStr );
        result = tkStr; 
        return true;
      } // if
    } // if

    return false;
  } // C_statement()

  bool Dec( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    int o_gAsize = gA_Var.size();
    if ( Type_specifier( tkStr, result ) ) {
      string type = tkStr[0].name;
      tkStr = result;
      gdec.g = false;
      gdec.type = type;
      gdec.fname = gfunction.name;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        gdec.name = tkStr[0].name;
        Get( tkStr );
        result = tkStr;
        if ( Rest_of_dec( tkStr, result, false ) ) {
          return true;
        } // if
      } // if
    } // if

    return false;
  } // Dec()

  bool Satement( vector<Ttk> tkStr,  vector<Ttk> & result, bool print ) {
    bool isDone = false;
    string nm;
    result = tkStr;
    if ( IsFive_Fname( tkStr[0].name ) ) {
      print = false;
      isDone = true;
      nm = tkStr[0].name;
    } // if

    if ( tkStr.size() > 0 && tkStr[0].type == ";" ) {
      Get( tkStr );
      result = tkStr;
      if ( print ) {
        gans.push_back( "Statement executed ..." );
      } // if

      return true;
    } // if
    else if ( Exp( tkStr, result ) ) {
      vector<Ttk> t = tkStr;

      tkStr = result;
      if ( tkStr.size() > 0 && tkStr[0].type == ";" ) {
        Get( tkStr );
        result = tkStr;
        if ( print ) { 
          gans.push_back( "Statement executed ..." );
        } // if

        if ( isDone ) {
          F f;
          f.name = nm;
          gA_Function.push_back( f );

          if ( gfunction.name != "" ) {
            Five_F( t, result );
          } // if
        } // if


        return true;
      } // if
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && tkStr[0].type == "return" ) {
      Get( tkStr );
      result = tkStr;
      if ( Exp( tkStr, result ) ) {
        tkStr = result;
      } // if

      if ( tkStr.size() > 0 && tkStr[0].type == ";" ) {
        Get( tkStr );
        result = tkStr;
        if ( print ) {
          gans.push_back( "Statement executed ..." );
        } // if

        return true;
      } // if
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && C_statement( tkStr, result ) ) {
      if ( print ) {
        gans.push_back( "Statement executed ..." );
      } // if

      return true;
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && tkStr[0].type == "if" ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
        Get( tkStr );
        result = tkStr;
        if ( Exp( tkStr, result ) ) {
          tkStr = result;
          if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
            Get( tkStr );
            result = tkStr;
            if ( Satement( tkStr, result, false ) ) {
              tkStr = result;
              if ( tkStr.size() > 0 && tkStr[0].type == "else" ) {
                Get( tkStr );
                result = tkStr;
                if ( !Satement( tkStr, result, false ) ) {      
                  return false;
                } // if
              } // if

              if ( print ) {
                gans.push_back( "Statement executed ..." ); 
              } // if 
    
              return true;
            } // if
            else {
              return false;
            } // else
          } // if
        } // if
        else {
          return false;
        } // else
      } // if
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && tkStr[0].type == "while" ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
        Get( tkStr );
        result = tkStr;
        if ( Exp( tkStr, result ) ) {
          tkStr = result;
          if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
            Get( tkStr );
            result = tkStr;
            if ( Satement( tkStr, result, false ) ) {
 
              if ( print ) {
                gans.push_back( "Statement executed ..." );
              } // if

              return true;

            } // if
            else {
              return false;
            } // else
          } // if
        } // if
        else {
          return false;
        } // else
      } // if    
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && tkStr[0].type == "do" ) {
      Get( tkStr );
      result = tkStr;
      if ( Satement( tkStr, result, false ) ) {
        tkStr = result;
        if ( tkStr.size() > 0 && tkStr[0].type == "while" ) {
          Get( tkStr );
          result = tkStr;
          if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
            Get( tkStr );
            result = tkStr;
            if ( Exp( tkStr, result ) ) {
              tkStr = result;
              if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
                Get( tkStr );
                result = tkStr;
                if ( tkStr.size() > 0 && tkStr[0].type == ";" ) {
                  Get( tkStr );
                  result = tkStr;
 
                  if ( print ) {
                    gans.push_back( "Statement executed ..." );
                  } // if

                  return true;
                } // if
              } // if
            } // if
            else {
              return false;
            } // else
          } // if 
        } // if 
      } // if 
      else {
        return false;
      } // else   
    } // else if

    return false;
  } // Satement()

  bool Exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Bc_exp( tkStr, result ) ) {

      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "," ) {
        Get( tkStr );
        result = tkStr;
        if ( !Bc_exp( tkStr, result ) ) {
          return false; 
        } // if
        else {
          tkStr = result;
        } // else
      } // while
 
      return true;
    } // if

    return false;
  } // Exp()

  bool Bc_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
      if ( IsUndefined( tkStr[0] ) ) {
        result = tkStr;       
        return false;
      } // if
      else {
        Get( tkStr );
        result = tkStr; 
      } // else

      if ( Rest_of_id_started_bc_exp( tkStr, result ) ) {
        return true;
      } // if
      else {
        return false;
      } // else
    } // if
    else if ( tkStr.size() > 0 && ( tkStr[0].type == "pp" || tkStr[0].type == "mm" ) ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        if ( IsUndefined( tkStr[0] ) ) {
          result = tkStr;         
          return false;
        } // if
        else {
          Get( tkStr );
          result = tkStr; 
        } // else

        if ( Rest_of_PPMM_id_started_bc_exp( tkStr, result ) ) {
          return true;
        } // if
        else {
          return false;
        } // else
      } // if
    } // else if
    else if ( Sign( tkStr, result ) ) {
      tkStr = result;
      while ( Sign( tkStr, result ) ) {
        tkStr = result;
      } // while

      if ( S_un_exp( tkStr, result ) ) {
        tkStr = result;
        if ( Romce_and_romloe( tkStr, result ) ) {
          return true;
        } // if
        else {
          return false;
        } // else      
      } // if
      else {
        return false;
      } // else
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() &&
              ( tkStr[0].type == "cst" || tkStr[0].type == "(" ) ) {
      if ( tkStr[0].type == "(" ) {
        Get( tkStr );
        result = tkStr;
        if ( Exp( tkStr, result ) ) {
          tkStr = result;
          if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
            Get( tkStr );
            result = tkStr;
            if ( Romce_and_romloe( tkStr, result ) ) {
              return true;
            } // if
            else {
              return false;
            } // else
          } // if
        } // if
        else {
          return false;
        } // else
      } // if
      else {
        Get( tkStr );
        result = tkStr;
        if ( Romce_and_romloe( tkStr, result ) ) {
          return true;
        } // if 
      } // else
    } // else if

    return false;
  } // Bc_exp()

  bool Rest_of_id_started_bc_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;

    if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
      Get( tkStr );
      result = tkStr;
      if ( Act_parlst( tkStr, result ) ) {
        tkStr = result;
      } // if

      if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
        Get( tkStr );
        result = tkStr;
        if ( Romce_and_romloe( tkStr, result ) ) {
          return true;
        } // if
        else {
          return false;
        } // else
      } // if      
    } // if
    else {
      if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
        Get( tkStr );
        result = tkStr;
        if ( Exp( tkStr, result ) ) {
          tkStr = result;
          if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
            Get( tkStr );
            result = tkStr;
          } // if
          else {
            return false;
          } // else
        } // if
        else {
          return false;
        } // else
      } // if

      if ( Assign_op( tkStr, result ) ) {
        tkStr = result;
        if ( Bc_exp( tkStr, result ) ) {
          return true;
        } // if
        else {
          return false;
        } // else 
      } // if
      else {
        if ( tkStr.size() > 0 && tkStr.size() == result.size() &&
             ( tkStr[0].type == "mm" || tkStr[0].type == "pp" ) ) {
          Get( tkStr );
          result = tkStr;
        } // if

        if ( tkStr.size() > 0 && tkStr.size() == result.size() &&
             Romce_and_romloe( tkStr, result ) ) {
          return true;
        } // if
        else {

          return false;
        } // else
      } // else
    } // else

    return false; 
  } // Rest_of_id_started_bc_exp()

  bool Rest_of_PPMM_id_started_bc_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
      Get( tkStr );
      result = tkStr;
      if ( Exp( tkStr, result ) ) {
        tkStr = result;
        if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
          Get( tkStr );
          result = tkStr;
        } // if
        else {
          return false;
        } // else
      } // if
      else {
        return false;
      } // else
    } // if

    if ( Romce_and_romloe( tkStr, result ) ) {
      return true;
    } // if
    else {
      return false;
    } // else
  } // Rest_of_PPMM_id_started_bc_exp()

  bool Sign( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    if ( tkStr[0].name == "+" || tkStr[0].name == "-" || tkStr[0].name == "!" ) {
      Get( tkStr );
      result = tkStr;
      return true;
    } // if

    result = tkStr;
    return false;
  } // Sign()

  bool Act_parlst( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Bc_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "," ) {
        Get( tkStr );
        result = tkStr;
        if ( !Bc_exp( tkStr, result ) ) {
          return false; 
        } // if
        else {
          tkStr = result;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Act_parlst()

  bool Assign_op( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr[0].type == "=" || tkStr[0].type == "te" || tkStr[0].type == "de" ||
         tkStr[0].type == "re" || tkStr[0].type == "pe" || tkStr[0].type == "me" ) {
      Get( tkStr );
      result = tkStr;
      return true;
    } // if

    return false;
  } // Assign_op()

  bool Romce_and_romloe( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Roml_or_exp( tkStr, result ) ) {
      tkStr = result;
      if ( tkStr.size() > 0 && tkStr[0].type == "?" ) {
        Get( tkStr );
        result = tkStr;
        if ( Bc_exp( tkStr, result ) ) {
          tkStr = result;
          if ( tkStr.size() > 0 && tkStr[0].type == ":" ) {
            Get( tkStr );
            result = tkStr;
            if ( Bc_exp( tkStr, result ) ) {
              return true;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // if
        else {
          return false;
        } // else
      } // if

      return true; 
    } // if

    return false;   
  } // Romce_and_romloe()

  bool Roml_or_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {

    result = tkStr;
    if ( Roml_and_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "or" ) {
        Get( tkStr );
        result = tkStr;
        if ( Ml_and_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Roml_or_exp()

  bool Ml_and_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Mb_or_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "and" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mb_or_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Ml_and_exp()

  bool Roml_and_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {

    result = tkStr;
    if ( Romb_or_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "and" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mb_or_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Roml_and_exp()

  bool Mb_or_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Mbe_or_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "|" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mbe_or_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {

          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Mb_or_exp()

  bool Romb_or_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rombe_or_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "|" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mbe_or_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if


    return false;
  } // Romb_or_exp()

  bool Mbe_or_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Mb_and_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "^" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mb_and_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Mbe_or_exp()

  bool Rombe_or_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Romb_and_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "^" ) {
        Get( tkStr );
        result = tkStr;
        if ( Mb_and_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Rombe_or_exp()

  bool Mb_and_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( M_eq_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "&" ) {
        Get( tkStr );
        result = tkStr;
        if ( M_eq_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
    } // if

    return false;
  } // Mb_and_exp()

  bool Romb_and_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rom_eq_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && tkStr[0].type == "&" ) {
        Get( tkStr );
        result = tkStr;
        if ( M_eq_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else
      } // while

      return true;
 
    } // if

    return false;
  } // Romb_and_exp()

  bool M_eq_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( M_rel_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "eq" || tkStr[0].type == "neq" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_rel_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // M_eq_exp()

  bool Rom_eq_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rom_rel_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "eq" || tkStr[0].type == "neq" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_rel_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // Rom_eq_exp()

  bool M_rel_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( M_s_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "<" || tkStr[0].type == ">" ||
                                    tkStr[0].type == "le" || tkStr[0].type == "ge" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_s_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // M_rel_exp()

  bool Rom_rel_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rom_s_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "<" || tkStr[0].type == ">" ||
                                    tkStr[0].type == "le" || tkStr[0].type == "ge" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_s_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // Rom_rel_exp()

  bool M_s_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( M_add_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "ls" || tkStr[0].type == "rs" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_add_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // M_s_exp()

  bool Rom_s_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rom_add_exp( tkStr, result ) ) {
      tkStr = result; 
      while ( tkStr.size() > 0 && ( tkStr[0].type == "ls" || tkStr[0].type == "rs" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_add_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // Rom_s_exp()

  bool M_add_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( M_mult_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "+" || tkStr[0].type == "-" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_mult_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // M_add_exp()

  bool Rom_add_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Rom_mult_exp( tkStr, result ) ) {
      tkStr = result;
      while ( tkStr.size() > 0 && ( tkStr[0].type == "+" || tkStr[0].type == "-" ) ) {
        Get( tkStr );
        result = tkStr;
        if ( M_mult_exp( tkStr, result ) ) {
          tkStr = result;
        } // if
        else {
          return false;
        } // else 
      } // while

      return true;
    } // if

    return false;
  } // Rom_add_exp()

  bool M_mult_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Un_exp( tkStr, result ) ) {
      tkStr = result;
      if ( Rom_mult_exp( tkStr, result ) ) {
        return true;
      } // if
      else {
        return false;
      } // else    
    } // if

    return false;
  } // M_mult_exp()

  bool Rom_mult_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    while ( tkStr.size() > 0 && ( tkStr[0].type == "*" || tkStr[0].type == "/" ||
                                  tkStr[0].type == "%" ) ) {
      Get( tkStr );
      result = tkStr;
      if ( Un_exp( tkStr, result ) ) {
        tkStr = result;
      } // if
      else {
        return false;
      } // else 
    } // while
  
    return true;
  } // Rom_mult_exp()

  bool Un_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( Sign( tkStr, result ) ) {
      tkStr = result;
      while ( Sign( tkStr, result ) ) {
        tkStr = result;
      } // while

      if ( tkStr.size() > 0 && tkStr.size() == result.size() && S_un_exp( tkStr, result ) ) {
        return true;
      } // if
      else {
        return false;
      } // else
    } // if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() && U_un_exp( tkStr, result ) ) {
      return true;
    } // else if
    else if ( tkStr.size() > 0 && tkStr.size() == result.size() &&
              ( tkStr[0].type == "pp" || tkStr[0].type == "mm" ) ) {
      Get( tkStr );
      result = tkStr;
      if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
        if ( IsUndefined( tkStr[0] ) ) {
          result = tkStr;         
          return false;
        } // if
        else {
          Get( tkStr ); 
          result = tkStr;
        } // else

        if ( tkStr.size() > 0 && tkStr[0].type == "[" ) {
          Get( tkStr );
          result = tkStr;
          if ( Exp( tkStr, result ) ) {
            tkStr = result;
            if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
              Get( tkStr );
              result = tkStr;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // if

        return true;
      } // if
    } // else if


    return false;
  } // Un_exp()

  bool S_un_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
      if ( IsUndefined( tkStr[0] ) ) {
        result = tkStr;         
        return false;
      } // if
      else {
        Get( tkStr ); 
        result = tkStr;
      } // else

      if ( tkStr.size() > 0 && ( tkStr[0].type == "[" || tkStr[0].type == "(" ) ) {
        if ( tkStr[0].type == "(" ) {
          Get( tkStr );
          result = tkStr;
          if ( Act_parlst( tkStr, result ) ) {
            tkStr = result;
          } // if

          if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
            Get( tkStr );
            result = tkStr;
            return true;
          } // if
          else {
            return false;
          } // else
        } // if
        else {
          Get( tkStr );
          result = tkStr;
          if ( Exp( tkStr, result ) ) {
            tkStr = result;
            if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
              Get( tkStr );
              result = tkStr;
              return true;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // else
      } // if

      return true;
    } // if
    else if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
      Get( tkStr );
      result = tkStr;
      return true;
    } // else if
    else if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
      Get( tkStr );
      result = tkStr;
      if ( Exp( tkStr, result ) ) {
        tkStr = result;
        if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
          Get( tkStr );
          result = tkStr;
          return true;
        } // if
        else {
          return false;
        } // else
      } // if
      else {
        return false;
      } // else
    } // else if


    return false;
  } // S_un_exp()

  bool U_un_exp( vector<Ttk> tkStr,  vector<Ttk> & result ) {
    bool in = false;
    result = tkStr;
    if ( tkStr.size() > 0 && tkStr[0].type == "id" ) {
      if ( IsUndefined( tkStr[0] ) ) {
        result = tkStr;         
        return false;
      } // if
      else {
        Get( tkStr );
        result = tkStr; 
      } // else

      if ( tkStr.size() > 0 && ( tkStr[0].type == "[" || tkStr[0].type == "(" ) ) {
        if ( tkStr[0].type == "(" ) {
          Get( tkStr );
          result = tkStr;
          if ( Act_parlst( tkStr, result ) ) {
            tkStr = result;
          } // if

          if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
            Get( tkStr );
            result = tkStr;
            return true;
          } // if
          else {
            return false;
          } // else
        } // if
        else {
          in = true;
          Get( tkStr );
          result = tkStr;
          if ( Exp( tkStr, result ) ) {
            tkStr = result;
            if ( tkStr.size() > 0 && tkStr[0].type == "]" ) {
              Get( tkStr );
              result = tkStr;
              return true;
            } // if
            else {
              return false;
            } // else
          } // if
          else {
            return false;
          } // else
        } // else
      } // if

      if ( in && tkStr.size() > 0 && ( tkStr[0].type == "pp" || tkStr[0].type == "mm" ) ) {
        Get( tkStr );
        result = tkStr;
      } // if

      return true;
    } // if
    else if ( tkStr.size() > 0 && tkStr[0].type == "cst" ) {
      Get( tkStr );
      result = tkStr;
      return true;
    } // else if
    else if ( tkStr.size() > 0 && tkStr[0].type == "(" ) {
      Get( tkStr );
      result = tkStr;
      if ( Exp( tkStr, result ) ) {
        tkStr = result;
        if ( tkStr.size() > 0 && tkStr[0].type == ")" ) {
          Get( tkStr );
          result = tkStr;
          return true;
        } // if
        else {
          return false;
        } // else
      } // if
      else {
        return false;
      } // else
    } // else if


    return false;
  } // U_un_exp()
};


int main() {
  vector<Ttk> allTk;
  vector<Ttk> tkTmp;
  vector<Ttk> result;
  gRow = 0;
  bool eof = false;
  string curLine;
  vector<string> glines;
  char testnum ;
  char testnum1;
  scanf( "%c", &testnum1 );
  scanf( "%c", &testnum );

  cout << "Our-C running ..." << endl;
  Scanner s;
  Parser p;
  p.mrec = 0;
  p.mll = true;
  /*
  if ( testnum1 == '3' ) {
    cout << "qq"; 
    exit( 0 );
  } // if
  */
  curLine = ReadLine( eof ) ;  
  while ( !eof ) {
    glines.push_back( curLine );
    gRow++;
    tkTmp = s.GetTk( curLine );
    for ( int i = 0; i < tkTmp.size() ; i++ ) {
      allTk.push_back( tkTmp[i] );
    } // for

    curLine = ReadLine( eof ) ;
  } // while()


  while ( allTk.size() > 0 ) {
    if ( !p.UserInput( allTk,  result ) ) {
      if ( result[0].type == "err1" ) {
        if ( p.mll ) {
          cout << "> ";
        } // if

        cout << "Line " << result[0].row - p.mrec << " : unrecognized token with first char : '";
        cout << glines[result[0].row-1][result[0].col-1] << "'" << endl;
        int skipLine = result[0].row;
    
        p.mrec = skipLine;
        for ( int i = 0; i < result.size() ; i++ ) {
          if ( result[i].row <= skipLine ) {
            result.erase( result.begin() + i );
            i--;
          } // if
        } // for

        if ( skipLine != result[0].row ) {
          p.mll = true;
        } // if
        else {
          p.mll = false;
        } // else
      } // if
      else if ( !gerr3 ) {
        if ( p.mll ) {
          cout << "> ";
        } // if

        cout << "Line " << result[0].row - p.mrec << " : unexpected token : '" ;
        cout << result[0].name << "'" << endl; 
        int skipLine = result[0].row;

        p.mrec = skipLine;
        for ( int i = 0; i < result.size() ; i++ ) {
          if ( result[i].row <= skipLine ) {
            result.erase( result.begin() + i );
            i--;
          } // if
        } // for 

        if ( skipLine != result[0].row ) {
          p.mll = true;
        } // if
        else {
          p.mll = false;
        } // else
      } // else if
      else {
        for ( int j = 0 ; j < result.size() ; j++ ) {
          if ( result[j].row <= p.mrec ) {
            result.erase( result.begin() + j );
            j--;
          } // if
        } // for 

        gerr3 = false;
      } // else
    } // if


    allTk = result;
  } // while()

  cout << "> Our-C exited ...";
  return 0;
} // main()


