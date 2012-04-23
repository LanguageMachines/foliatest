#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <unicode/unistr.h>
#include "libxml/tree.h"
#include "libfolia/document.h"
#include "libfolia/folia.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;
using namespace folia;

class foliaTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( foliaTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST( test2 );
  CPPUNIT_TEST( test3 );
  CPPUNIT_TEST( test4 );
  CPPUNIT_TEST( test5 );
  CPPUNIT_TEST_SUITE_END();
protected:
  void test0();
  void test1();
  void test2();
  void test3();
  void test4();
  void test5();
};

void foliaTest::test0() {
  cout << " Test lezen van KWargs ";
  KWargs bla;
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "dit='goed', dat =' ra ar' " ) );
  CPPUNIT_ASSERT ( bla["dit"] == "goed" );
  CPPUNIT_ASSERT ( bla["dat"] == " ra ar" );
  CPPUNIT_ASSERT_EQUAL ( bla["dat"] , string(" ra ar") );
  CPPUNIT_ASSERT_THROW( bla = getArgs( "dit='fout' = " ), ArgsError );
  CPPUNIT_ASSERT_THROW( bla = getArgs( "dit=fout" ), ArgsError );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='o\\'k'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "o'k" );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='o\\k'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "o\\k" );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='o\"k'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "o\"k" );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='o""k'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "ok" );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='ok\\a', bli='bla'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "ok\\a" );
  CPPUNIT_ASSERT ( bla["bli"] == "bla" );
  CPPUNIT_ASSERT_NO_THROW( bla = getArgs( "cls='ok\\\\', bli='bla'" ) );
  CPPUNIT_ASSERT ( bla["cls"] == "ok\\" );
  CPPUNIT_ASSERT ( bla["bli"] == "bla" );
}


void foliaTest::test1() {
  cout << " Test lezen van een FoLiA file ";
  Document d;
  CPPUNIT_ASSERT_NO_THROW( d.readFromFile( "tests/folia.example" ) );
}


void foliaTest::test2() {
  cout << " Test lezen van een FoLiA string ";
  string s;
  ifstream is( "tests/folia.example" );
  string line;
  while( getline( is, line ) ){
    s += line + "\n";
  };
  Document d;
  CPPUNIT_ASSERT_NO_THROW( d.readFromString( s ) );
}

void foliaTest::test3() {
  cout << " Test lezen van een DCOI file ";
  Document d;
  CPPUNIT_ASSERT_THROW( d.readFromFile( "tests/dcoi.example" ), XmlError );
}

void foliaTest::test4() {
  cout << " Test uitvoer van een FoLiA file naar string";
  Document d;
  CPPUNIT_ASSERT_NO_THROW( d.readFromFile( "tests/folia.example" ) );
  string out;
  CPPUNIT_ASSERT_NO_THROW( out = d.toXml() );
}

void foliaTest::test5() {
  cout << " Test lezen van een FoLiA file met namespaces";
  Document d;
  CPPUNIT_ASSERT_NO_THROW( d.readFromFile( "tests/folia.nsexample" ) );
  CPPUNIT_ASSERT_NO_THROW( d.save( "/tmp/test5.out", "fl" ) );
  int stat = system( "xmldiff /tmp/test5.out tests/folia.nsexample" );
  CPPUNIT_ASSERT_MESSAGE( "/tmp/test5.out tests/folia.nsexample differ!",
			  (stat == 0) );
}

class sanityTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( sanityTest );
  CPPUNIT_TEST( test000 );
  CPPUNIT_TEST( test001 );
  CPPUNIT_TEST( test002 );
  CPPUNIT_TEST( test003 );
  CPPUNIT_TEST( test004 );
  CPPUNIT_TEST( test005 );
  CPPUNIT_TEST( test006 );
  CPPUNIT_TEST( test007 );
  CPPUNIT_TEST( test008 );
  CPPUNIT_TEST( test009 );
  CPPUNIT_TEST( test010 );
  CPPUNIT_TEST( test011 );
  CPPUNIT_TEST( test012 );
  CPPUNIT_TEST( test013 );
  CPPUNIT_TEST( test014 );
  CPPUNIT_TEST( test015 );
  CPPUNIT_TEST( test016a );
  CPPUNIT_TEST( test016b );
  CPPUNIT_TEST( test017 );
  CPPUNIT_TEST( test018 );
  CPPUNIT_TEST( test019 );
  CPPUNIT_TEST( test020a );
  CPPUNIT_TEST( test020b );
  CPPUNIT_TEST( test020c );
  CPPUNIT_TEST( test020d );
  CPPUNIT_TEST( test021 );
  CPPUNIT_TEST( test022 );
  CPPUNIT_TEST( test023a );
  CPPUNIT_TEST( test023b );
  CPPUNIT_TEST( test023c );
  CPPUNIT_TEST( test024a );
  CPPUNIT_TEST( test024b );
  CPPUNIT_TEST( test024c );
  CPPUNIT_TEST( test025a );
  CPPUNIT_TEST( test025b );
  CPPUNIT_TEST( test025c );
  CPPUNIT_TEST( test026 );
  CPPUNIT_TEST( test027 );
  CPPUNIT_TEST( test028 );
  CPPUNIT_TEST( test029 );
  CPPUNIT_TEST( test030 );
  CPPUNIT_TEST( test031 );
  CPPUNIT_TEST( test032 );
  CPPUNIT_TEST( test033 );
  CPPUNIT_TEST( test034 );
  CPPUNIT_TEST( test035 );
  CPPUNIT_TEST( test036 );
  CPPUNIT_TEST( test037 );
  CPPUNIT_TEST( test099 );
  CPPUNIT_TEST( test100a );
  CPPUNIT_TEST( test100b );
  CPPUNIT_TEST( test101 );
  CPPUNIT_TEST( test101a );
  CPPUNIT_TEST( test101b );
  CPPUNIT_TEST( test102 );
  CPPUNIT_TEST( test102a );
  CPPUNIT_TEST( test102b );
  CPPUNIT_TEST( test102c );
  CPPUNIT_TEST( test102d1 );
  CPPUNIT_TEST( test102d2 );
  CPPUNIT_TEST( test102d3 );
  CPPUNIT_TEST( test102e );
  CPPUNIT_TEST( test102f );
  CPPUNIT_TEST( test102g );
  CPPUNIT_TEST( test102h );
  CPPUNIT_TEST( test102i );
  CPPUNIT_TEST( test102j );
  CPPUNIT_TEST( test102k );
  CPPUNIT_TEST( test103 );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
protected:
  void test000();
  void test001();
  void test002();
  void test003();
  void test004();
  void test005();
  void test006();
  void test007();
  void test008();
  void test009();
  void test010();
  void test011();
  void test012();
  void test013();
  void test014();
  void test015();
  void test016a();
  void test016b();
  void test017();
  void test018();
  void test019();
  void test020a();
  void test020b();
  void test020c();
  void test020d();
  void test021();
  void test022();
  void test023a();
  void test023b();
  void test023c();
  void test024a();
  void test024b();
  void test024c();
  void test025a();
  void test025b();
  void test025c();
  void test026();
  void test027();
  void test028();
  void test029();
  void test030();
  void test031();
  void test032();
  void test033();
  void test034();
  void test035();
  void test036();
  void test037();
  void test099();
  void test100a();
  void test100b();
  void test101();
  void test101a();
  void test101b();
  void test102();
  void test102a();
  void test102b();
  void test102c();
  void test102d1();
  void test102d2();
  void test102d3();
  void test102e();
  void test102f();
  void test102g();
  void test102h();
  void test102i();
  void test102j();
  void test102k();
  void test103();
  Document doc;
};


void sanityTest::setUp( ){
  doc.readFromFile( "tests/folia.example" );
}

void sanityTest::test000( ){
  cout << " Text count ";
  CPPUNIT_ASSERT_EQUAL( doc.size(), 1 );
  CPPUNIT_ASSERT( doc[0]->isinstance<Text>() );
}

void sanityTest::test001( ){
  cout << " Paragraph count ";
  CPPUNIT_ASSERT( doc.paragraphs().size() == 1 );
}

void sanityTest::test002(){
  cout << " Sentence count ";
  vector<Sentence*> v;
  CPPUNIT_ASSERT_NO_THROW( v = doc.sentences() );
  CPPUNIT_ASSERT_EQUAL( v.size(), size_t(12) );
}

void sanityTest::test003( ){
  cout << " Word count ";
  vector<Word*> v;
  CPPUNIT_ASSERT_NO_THROW( v = doc.words() );
  CPPUNIT_ASSERT_EQUAL( v.size(), (size_t)157 );
}

void sanityTest::test004(){
  cout << " first word ";
  FoliaElement* w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.words(0) );
  CPPUNIT_ASSERT( w->isinstance( Word_t ) );
  CPPUNIT_ASSERT( w->id() == "WR-P-E-J-0000000001.head.1.s.1.w.1" );
  CPPUNIT_ASSERT( w->text() == "Stemma" );
  CPPUNIT_ASSERT( str(w) == "Stemma" );
  CPPUNIT_ASSERT( unicode(w) == "Stemma" );
}

void sanityTest::test005( ){
  cout << " last word ";
  FoliaElement* w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.rwords(0) );
  CPPUNIT_ASSERT( w->isinstance<Word>() );
  CPPUNIT_ASSERT_EQUAL( w->id(), string("sandbox.figure.1.caption.s.1.w.2") );
  CPPUNIT_ASSERT_EQUAL( w->text(),  UnicodeString("stamboom") );
  CPPUNIT_ASSERT_EQUAL( str(w), string("stamboom") );
}

void sanityTest::test006( ){
  cout << " second sentence ";
  FoliaElement* w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.sentences(1) );
  CPPUNIT_ASSERT( isinstance( w, Sentence_t ) );
  CPPUNIT_ASSERT( w->id() == "WR-P-E-J-0000000001.p.1.s.1" );
  CPPUNIT_ASSERT( !w->hastext() );
  CPPUNIT_ASSERT( w->str() == "Stemma is een ander woord voor stamboom ." );
}

void sanityTest::test007( ){
  cout << " use index ";
  FoliaElement *e = 0;
  CPPUNIT_ASSERT_NO_THROW( e = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"] );
  CPPUNIT_ASSERT( e->isinstance( Word_t ) );
  CPPUNIT_ASSERT_EQUAL( e, doc.index("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  CPPUNIT_ASSERT_EQUAL( e->id(), string("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  CPPUNIT_ASSERT( e->text() == "stamboom" );
}

void sanityTest::test008(){
  cout << " division + head ";
  FoliaElement *e = 0;
  CPPUNIT_ASSERT_NO_THROW( e = doc["WR-P-E-J-0000000001.div0.1"] );
  CPPUNIT_ASSERT( e->isinstance( Division_t ) );
  FoliaElement *h;
  CPPUNIT_ASSERT_NO_THROW( h = e->head() );
  CPPUNIT_ASSERT_EQUAL( h,  doc["WR-P-E-J-0000000001.head.1"] );
  CPPUNIT_ASSERT( len( h ) == 1 );
}

void sanityTest::test009( ){
  cout << " Token Annotation - Pos ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.words(0) );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>() == w->select<PosAnnotation>()[0] );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>()->isinstance(Pos_t ) );
  //  CPPUNIT_ASSERT( issubclass( PosAnnotation, AbstractTokenAnnotation ) );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>()->cls() == "N(soort,ev,basis,onz,stan)" );
  CPPUNIT_ASSERT( w->pos() == "N(soort,ev,basis,onz,stan)" );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>()->sett() == "cgn-combinedtags" );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>()->annotator() == "tadpole" );
  CPPUNIT_ASSERT( w->annotation<PosAnnotation>()->annotatortype() == AUTO );
}

void sanityTest::test010( ){
  cout << " Token Annotation - Lemma ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.words(0) );
  FoliaElement *l = 0;
  CPPUNIT_ASSERT_NO_THROW( l = w->annotation<LemmaAnnotation>() );
  CPPUNIT_ASSERT( l == w->select<LemmaAnnotation>()[0] );
  CPPUNIT_ASSERT( isinstance( l, Lemma_t ) );
  CPPUNIT_ASSERT( l->cls() == "stemma" );
  CPPUNIT_ASSERT( w->lemma() == "stemma" );
  CPPUNIT_ASSERT( l->sett() == "lemmas-nl" );
  CPPUNIT_ASSERT( l->annotator() == "tadpole" );
  CPPUNIT_ASSERT( l->annotatortype() == AUTO );
}

void sanityTest::test011( ){
  cout << " Token Annotation - Non-existing element ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc.words(0) );
  CPPUNIT_ASSERT( len( w->select<SenseAnnotation>() ) == 0 );
  CPPUNIT_ASSERT_THROW( w->annotation<SenseAnnotation>(), 
			NoSuchAnnotation );
}

void sanityTest::test012( ){
  cout << " Correction - Text ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.6.w.31"] );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>( ) );
  CPPUNIT_ASSERT( len( c->getNew() ) == 1 );
  CPPUNIT_ASSERT( len( c->getOriginal() ) == 1 );
  CPPUNIT_ASSERT( w->text() == "vierkante" );
  CPPUNIT_ASSERT( c->getNew()->index(0)->text() == "vierkante" );
  CPPUNIT_ASSERT( c->getOriginal()->index(0)->text() == "vierkant" );
}

void sanityTest::test013( ){
  cout << " Correction - Token Annotation ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.6.w.32"] );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>( ) );
  CPPUNIT_ASSERT( len( c->getNew() ) == 1 );
  CPPUNIT_ASSERT( len( c->getOriginal() ) == 1 );
  CPPUNIT_ASSERT( w->annotation<LemmaAnnotation>()->cls() == "haak" );
  CPPUNIT_ASSERT( (*c->getNew())[0]->cls() == "haak" );
  CPPUNIT_ASSERT( (*c->getOriginal())[0]->cls() == "haaak" );
}

void sanityTest::test014(){
  cout << " Correction - Suggestions (text) ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.14"] );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>( ) );
  CPPUNIT_ASSERT( c->suggestions().size() == 2 );
  CPPUNIT_ASSERT( c->suggestions(0)->text() == "twijfelachtige" );
  CPPUNIT_ASSERT( c->suggestions(1)->text() == "ongewisse" );
}

string repr( FoliaElement *a ){
  if ( a )
    return "[" + toString( a->element_id() ) + "]";
  else
    return "[null]";
}

void check( FoliaElement *parent, const string& indent, ostream& os ){
  for ( size_t i=0; i < parent->size(); ++i ){
    FoliaElement *child = parent->index(i);
    os << indent << repr( child ) << endl;
    if ( ! ( (parent->isinstance( SyntacticUnit_t ) 
	      || parent->isinstance( Chunk_t ) 
	      || parent->isinstance( Entity_t ) 
	      || parent->isinstance( DependencyHead_t )
	      || parent->isinstance( DependencyDependent_t ) ) 
	     && child->isinstance( Word_t ) ) ){
      CPPUNIT_ASSERT( parent == child->parent() );
      check( child, indent + " ", os );
    }
  }
}

void sanityTest::test015( ){
  cout << " Checking if all elements know who's their daddy ";
  ofstream os( "/tmp/foliaparent.txt" );
  CPPUNIT_ASSERT_NO_THROW( check( doc.doc(), "",  os ) );
  int stat = system( "diff -b /tmp/foliaparent.txt tests/foliaparent.ok" );
  CPPUNIT_ASSERT_MESSAGE( "/tmp/foliaparent.txt tests/foliaparent.ok differ!", stat == 0 );
}


void sanityTest::test016a(){
  cout << " Check - Description ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.1.w.6"];
  CPPUNIT_ASSERT( w->description() == "Dit woordje is een voorzetsel, het is maar dat je het weet..." );
}

void sanityTest::test016b(){
  cout << " Check - Error on non-existing description ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.1.w.7"];
  CPPUNIT_ASSERT_THROW( w->description(), NoDescription );
}

void sanityTest::test017(){
  cout << " Check - Gap ";
  FoliaElement *gap = doc["WR-P-E-J-0000000001.gap.1"];
  CPPUNIT_ASSERT_EQUAL( strip(gap->content()).substr(0,11), string("De tekst is") );
  CPPUNIT_ASSERT( gap->cls() == "backmatter" );
  CPPUNIT_ASSERT( gap->description() == "Backmatter" );
}

void sanityTest::test018(){
  cout << " Subtoken annotation (morphological) ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.3.w.5"];
  FoliaElement *m = 0;
  CPPUNIT_ASSERT_NO_THROW( m = w->annotation<MorphologyLayer>() );
  CPPUNIT_ASSERT( len(m) == 2 );
  CPPUNIT_ASSERT( m->index(0)->isinstance( Morpheme_t ) );
  CPPUNIT_ASSERT( m->index(0)->text() == "handschrift" );
  CPPUNIT_ASSERT( m->index(0)->feat("type") == "stem" );
  CPPUNIT_ASSERT( m->index(0)->feat("function") == "lexical" );
  CPPUNIT_ASSERT( m->index(1)->text() == "en" );
  CPPUNIT_ASSERT( m->index(1)->feat("type") == "suffix" );
  CPPUNIT_ASSERT( m->index(1)->feat("function") == "plural" );
}

void sanityTest::test019(){
  cout << " Alignment ";
  throw NotImplementedError("test019");
}

void sanityTest::test020a(){
  cout << " Span annotation (Syntax) ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.1"];
  FoliaElement *l = 0;
  CPPUNIT_ASSERT_NO_THROW( l = s->annotation<SyntaxLayer>() );
  CPPUNIT_ASSERT( isinstance( l->index(0), SyntacticUnit_t ) );
  CPPUNIT_ASSERT( l->index(0)->cls() == "sentence" );
  CPPUNIT_ASSERT( l->index(0)->index(0)->cls() == "subject" );
  CPPUNIT_ASSERT( l->index(0)->index(0)->text() == "Stemma" );
  CPPUNIT_ASSERT( l->index(0)->index(1)->cls() == "verb" );
  CPPUNIT_ASSERT( l->index(0)->index(2)->cls() == "predicate" );
  CPPUNIT_ASSERT( l->index(0)->index(2)->index(0)->cls() == "np" );
  CPPUNIT_ASSERT( l->index(0)->index(2)->index(1)->cls() == "pp" );
  CPPUNIT_ASSERT( l->index(0)->index(2)->index(1)->text() == "voor stamboom" );
  CPPUNIT_ASSERT( l->index(0)->index(2)->text() == "een ander woord voor stamboom" );
}

void sanityTest::test020b(){
  cout << " Span annotation (Chunking) ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.1"];
  ChunkingLayer *l = 0;
  CPPUNIT_ASSERT_NO_THROW( l = s->annotation<ChunkingLayer>() );
  CPPUNIT_ASSERT( isinstance( l->index(0), Chunk_t ) );
  CPPUNIT_ASSERT( l->index(0)->text() == "een ander woord" );
  CPPUNIT_ASSERT( l->index(1)->text() == "voor stamboom" );
}

void sanityTest::test020c(){
  cout << " Span annotation (Entities) ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.1"];
  EntitiesLayer *l = 0;
  CPPUNIT_ASSERT_NO_THROW( l = s->annotation<EntitiesLayer>() );
  CPPUNIT_ASSERT( isinstance( l->index(0), Entity_t ) );
  CPPUNIT_ASSERT( l->index(0)->text() == "ander woord" );
}

void sanityTest::test020d(){
  cout << " Span annotation (Dependencies) ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.1"];
  DependenciesLayer *l = 0;
  CPPUNIT_ASSERT_NO_THROW( l = s->annotation<DependenciesLayer>( ) );
  CPPUNIT_ASSERT( l->size() == 6 );

  CPPUNIT_ASSERT( l->index(0)->head()->text() == "is" );
  CPPUNIT_ASSERT( l->index(0)->dependent()->text() == "Stemma" );
  CPPUNIT_ASSERT( l->index(0)->cls() == "su" );

  CPPUNIT_ASSERT( l->index(1)->head()->text() == "is" );
  CPPUNIT_ASSERT( l->index(1)->dependent()->text() == "woord" );
  CPPUNIT_ASSERT( l->index(1)->cls() == "predc" );

  CPPUNIT_ASSERT( l->index(2)->head()->text() == "woord" );
  CPPUNIT_ASSERT( l->index(2)->dependent()->text() == "een" );
  CPPUNIT_ASSERT( l->index(2)->cls() == "det" );

  CPPUNIT_ASSERT( l->index(3)->head()->text() == "woord" );
  CPPUNIT_ASSERT( l->index(3)->dependent()->text() == "ander" );
  CPPUNIT_ASSERT( l->index(3)->cls() == "mod" );

  CPPUNIT_ASSERT( l->index(4)->head()->text() == "woord" );
  CPPUNIT_ASSERT( l->index(4)->dependent()->text() == "voor" );
  CPPUNIT_ASSERT( l->index(4)->cls() == "mod" );

  CPPUNIT_ASSERT( l->index(5)->head()->text() == "voor" );
  CPPUNIT_ASSERT( l->index(5)->dependent()->text() == "stamboom" );
  CPPUNIT_ASSERT( l->index(5)->cls() == "obj1" );
}

void sanityTest::test021(){
  cout << " Obtaining previous word ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *prevw = w->previous();
  CPPUNIT_ASSERT( prevw->isinstance( Word_t ) );
  CPPUNIT_ASSERT( prevw->text() == "zo'n" );
}

void sanityTest::test022(){
  cout << " Obtaining next word ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *prevw = w->next();
  CPPUNIT_ASSERT( prevw->isinstance( Word_t ) );
  CPPUNIT_ASSERT( prevw->text() == "," );
}

void sanityTest::test023a(){
  cout << " Obtaining left context ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->leftcontext(3);
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( context[0]->text() == "wetenschap" );
  CPPUNIT_ASSERT( context[1]->text() == "wordt" );
  CPPUNIT_ASSERT( context[2]->text() == "zo'n" );
}

void sanityTest::test023b(){
  cout << " Obtaining left context with default placeholder ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.1.w.2"];
  vector<Word*> context = w->leftcontext(3);
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( context[0] == 0 );
  CPPUNIT_ASSERT( context[1]->text() == "Stemma" );
  CPPUNIT_ASSERT( context[2]->text() == "Stemma" );
}

void sanityTest::test023c(){
  cout << " Obtaining left context with placeholder ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.1.w.2"];
  vector<Word*> context = w->leftcontext(3,"?");
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( context[0]->isinstance( PlaceHolder_t) );
  CPPUNIT_ASSERT( context[0]->text() == "?" );
  CPPUNIT_ASSERT( context[1]->text() == "Stemma" );
  CPPUNIT_ASSERT( context[2]->text() == "Stemma" );
}

void sanityTest::test024a(){
  cout << " Obtaining right context ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->rightcontext(3);
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( text(context[0]) == "," );
  CPPUNIT_ASSERT( text(context[1]) == "onder" );
  CPPUNIT_ASSERT( text(context[2]) == "de" );
}

void sanityTest::test024b(){
  cout << " Obtaining right context with default placeholder ";
  FoliaElement *w = doc["sandbox.figure.1.caption.s.1.w.1"];
  vector<Word*> context = w->rightcontext(3);
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( text(context[0]) == "stamboom" );
  CPPUNIT_ASSERT( context[1] == 0 );
  CPPUNIT_ASSERT( context[2] == 0 );
}

void sanityTest::test024c(){
  cout << " Obtaining right context with placeholder ";
  FoliaElement *w = doc["sandbox.figure.1.caption.s.1.w.1"];
  vector<Word*> context = w->rightcontext(3, "_");
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 3 );
  CPPUNIT_ASSERT( text(context[0]) == "stamboom" );
  CPPUNIT_ASSERT( context[1]->isinstance(PlaceHolder_t) );
  CPPUNIT_ASSERT( context[2]->text() == "_" );
}

void sanityTest::test025a(){
  cout << " Obtaining full context ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(3);
  //  cerr << "found context " << context << endl;
  CPPUNIT_ASSERT( context.size() == 7 );
  CPPUNIT_ASSERT( context[0]->text() == "wetenschap" );
  CPPUNIT_ASSERT( context[1]->text() == "wordt" );
  CPPUNIT_ASSERT( context[2]->text() == "zo'n" );
  CPPUNIT_ASSERT( text(context[3]) == "stamboom" );
  CPPUNIT_ASSERT( text(context[4]) == "," );
  CPPUNIT_ASSERT( text(context[5]) == "onder" );
  CPPUNIT_ASSERT( text(context[6]) == "de" );
}

void sanityTest::test025b(){
  cout << " Obtaining full context with default placeholder ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(150);
  CPPUNIT_ASSERT( context.size() == 301 );
  CPPUNIT_ASSERT( context[0] == 0 );
  CPPUNIT_ASSERT( text(context[202]) == "handschrift" );
  CPPUNIT_ASSERT( context[300] == 0 );
}

void sanityTest::test025c(){
  cout << " Obtaining full context with string placeholder ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(150, "nil");
  CPPUNIT_ASSERT( context.size() == 301 );
  CPPUNIT_ASSERT( text(context[0]) == "nil" );
  CPPUNIT_ASSERT( text(context[202]) == "handschrift" );
  CPPUNIT_ASSERT( context[300]->isinstance(PlaceHolder_t) );
}

void sanityTest::test026(){
  cout << " Features ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.6.w.1"];
  FoliaElement *pos = w->annotation<PosAnnotation>();
  CPPUNIT_ASSERT( pos->isinstance(Pos_t) );
  CPPUNIT_ASSERT( pos->cls() == "WW(vd,prenom,zonder)" );
  CPPUNIT_ASSERT( len(pos) ==  1 );
  vector<Feature*> features = pos->select<Feature>();
  CPPUNIT_ASSERT( len(features) == 1 );
  CPPUNIT_ASSERT( isinstance(features[0], Feature_t ) );
  CPPUNIT_ASSERT( features[0]->subset() == "head" );
  CPPUNIT_ASSERT( features[0]->cls() == "WW" );
}

void sanityTest::test027(){
  cout << " Time Stamp ";
  FoliaElement *word = doc["WR-P-E-J-0000000001.p.1.s.8.w.15"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  //  cerr << endl << "'" << pos->getDateTime() << "'" << endl;
  CPPUNIT_ASSERT( pos->getDateTime() == "2011-07-20T19:00:01" );
  //  cerr << endl << pos->xmlstring() << endl;
  CPPUNIT_ASSERT( pos->xmlstring() == "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\"/>" );      
}

void sanityTest::test028() {
  cout << " Finding parents of word ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.8.w.15"];
  
  FoliaElement *s = w->sentence();
  CPPUNIT_ASSERT( s->isinstance( Sentence_t ) );
  CPPUNIT_ASSERT( s->id() == "WR-P-E-J-0000000001.p.1.s.8" );
        
  FoliaElement *p = w->paragraph();
  CPPUNIT_ASSERT( isinstance( p, Paragraph_t ) );
  CPPUNIT_ASSERT( p->id() == "WR-P-E-J-0000000001.p.1" );
  
  FoliaElement *div = w->division();
  CPPUNIT_ASSERT( isinstance( div, Division_t ) );
  CPPUNIT_ASSERT( div->id() == "WR-P-E-J-0000000001.div0.1" );
  CPPUNIT_ASSERT( w->incorrection() == 0 );
}

void sanityTest::test029(){
  cout << " Sanity Check - Quote ";
  FoliaElement *q = doc["WR-P-E-J-0000000001.p.1.s.8.q.1"];
  CPPUNIT_ASSERT( q->isinstance( Quote_t ) );
  CPPUNIT_ASSERT( q->text() == "volle lijn" );
  
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.8"];
  CPPUNIT_ASSERT( s->text() == "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  // (spelling errors are present in sentence)
        
  // get a word from the quote
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.8.w.2"];
  // and check if sentence matches
  CPPUNIT_ASSERT( w->sentence() == s );
}

void sanityTest::test030( ){
  cout << " Text Content ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.6"];
  CPPUNIT_ASSERT( s->text() == "Verdwenen handschriften waarvan men toch vermoedt dat ze ooit bestaan hebben worden ook in het stemma opgenomen en worden weergegeven door de laatste letters van het alfabet en worden tussen vierkante haken geplaatst ." );
  CPPUNIT_ASSERT_THROW( s->stricttext(), NoSuchText );
  CPPUNIT_ASSERT( s->stricttext("original") == "Verdwenen handschriften waarvan men toch vermoedt dat ze ooit bestaan hebben worden ook in het stemma opgenomen en worden weergegeven door de laatste letters van het alfabet en worden tussen vierkant haken geplaatst ." );
  CPPUNIT_ASSERT_THROW( s->text( "BLAH" ), NoSuchText );

  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.4.w.2"];
  CPPUNIT_ASSERT( w->text() == "hoofdletter" );
  CPPUNIT_ASSERT( w->textcontent()->text() == "hoofdletter" );
  CPPUNIT_ASSERT( w->textcontent()->offset() == 3 );
  
  FoliaElement *w2= doc["WR-P-E-J-0000000001.p.1.s.6.w.31"];
  CPPUNIT_ASSERT( w2->text() == "vierkante" );
  CPPUNIT_ASSERT( w2->stricttext() == "vierkante" );
  CPPUNIT_ASSERT( w2->stricttext("original") == "vierkant" );
}

void sanityTest::test031( ){
  cout << " Lexical Semantic Sense Annotation ";
  FoliaElement *w = doc["sandbox.list.1.listitem.1.s.1.w.1"];
  FoliaElement *sense = w->annotation<SenseAnnotation>( );
  CPPUNIT_ASSERT( sense->cls() == "some.sense.id" );
  CPPUNIT_ASSERT( sense->feat("synset") == "some.synset.id" );
}

void sanityTest::test032( ){
  cout << " Events ";
  FoliaElement *l = doc["sandbox.list.1"];
  FoliaElement *event = l->annotation<Event>();
  CPPUNIT_ASSERT( event->cls() == "applause" );
  CPPUNIT_ASSERT( event->feat("actor") == "audience" );
}

void sanityTest::test033( ){
  cout << " List ";
  FoliaElement *l = doc["sandbox.list.1"];
  CPPUNIT_ASSERT( l->index(0)->isinstance<ListItem>() );
  CPPUNIT_ASSERT( l->index(0)->n() == "1" );
  CPPUNIT_ASSERT( l->index(0)->text() == "Eerste testitem" );
  CPPUNIT_ASSERT( l->rindex(0)->isinstance<ListItem>() );
  CPPUNIT_ASSERT( l->rindex(0)->n() == "2" );
  CPPUNIT_ASSERT( l->rindex(0)->text() == "Tweede testitem" );
}

void sanityTest::test034( ){
  cout << " Figure ";
  FoliaElement *fig = doc["sandbox.figure.1"];
  CPPUNIT_ASSERT( fig->src() == "http://upload.wikimedia.org/wikipedia/commons/8/8e/Family_tree.svg" );
  CPPUNIT_ASSERT( fig->caption() == "Een stamboom" );
}

void sanityTest::test035( ){
  cout << " Event ";
  FoliaElement *e = doc["sandbox.event.1"];
  CPPUNIT_ASSERT( e->feat("actor") == "proycon" );
  CPPUNIT_ASSERT( e->feat("begindatetime") == "2011-12-15T19:01" );
  CPPUNIT_ASSERT( e->feat("enddatetime") == "2011-12-15T19:05" );
}

void sanityTest::test036( ){
  cout << " Paragraph and Sentence annotation ";
  FoliaElement *e = doc["WR-P-E-J-0000000001.p.1"];
  CPPUNIT_ASSERT( e->cls() == "firstparagraph" );
  e = doc["WR-P-E-J-0000000001.p.1.s.6"];
  CPPUNIT_ASSERT( e->cls() == "sentence" );
}

void sanityTest::test037( ){
  cout << " Feature test & Ambiguitity resolution of head as PoS Feature and as structure element ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <pos-annotation set=\"test\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"test.text\">\n"
"    <div xml:id=\"div\">\n"
"     <head xml:id=\"head\">\n"
"       <s xml:id=\"head.1.s.1\">\n"
"            <w xml:id=\"head.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"NN(blah)\" head=\"NN\" />\n"
"            </w>\n"
"        </s>\n"
"    </head>\n"
"    <p xml:id=\"p.1\">\n"
"        <s xml:id=\"p.1.s.1\">\n"
"            <w xml:id=\"p.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"NN(blah)\">\n"
"                    <headfeature class=\"NN\" />\n"
"                </pos>\n"
"            </w>\n"
"        </s>\n"
"    </p>\n"
"    <p xml:id=\"p.2\">\n"
"        <s xml:id=\"p.2.s.1\">\n"
"            <w xml:id=\"p.2.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"BB(blah)\">\n"
"                    <feat subset=\"head\" class=\"BB\" />\n"
"                </pos>\n"
"            </w>\n"
"        </s>\n"
"    </p>\n"
"   </div>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( doc["head.1.s.1.w.1"]->pos() == "NN(blah)" );
  CPPUNIT_ASSERT( doc["head.1.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "NN" );
  CPPUNIT_ASSERT( doc["p.1.s.1.w.1"]->pos() == "NN(blah)" );
  CPPUNIT_ASSERT( doc["p.1.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "NN" );
  CPPUNIT_ASSERT( doc["p.2.s.1.w.1"]->pos() == "BB(blah)" );
  CPPUNIT_ASSERT( doc["p.2.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "BB" );
}

void sanityTest::test099(){
  cout << " Writing to file ";
  CPPUNIT_ASSERT_NO_THROW( doc.save( "/tmp/savetest.xml" ) );
}

void sanityTest::test100a( ){
  cout << " Checking saved file against document ";
  Document d;
  CPPUNIT_ASSERT_NO_THROW( d.readFromFile( "/tmp/savetest.xml" ) );
  CPPUNIT_ASSERT( d == doc );
}

void sanityTest::test100b( ){
  cout << " Checking saved file against input file ";
  int stat = system( "xmldiff /tmp/savetest.xml tests/folia.example" );
  CPPUNIT_ASSERT_MESSAGE( "/tmp/savetest.xml tests/folia.example differ!",
			  stat == 0 );
}

void sanityTest::test101( ){
  cout << " Add a sentence at wrong position ";
  FoliaElement *p = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *s = 0;
  CPPUNIT_ASSERT_NO_THROW( s = new Sentence( &doc, "generate_id='" + p->id() + "'" ) );
  CPPUNIT_ASSERT_THROW( p->append( s ), ValueError );
}

void sanityTest::test101a(){
  cout << " Metadata external reference (CMDI) ";
  Document doc = Document("file='tests/folia.cmdi.xml'");
  CPPUNIT_ASSERT( doc.metadatatype() == CMDI );
  CPPUNIT_ASSERT( doc.metadatafile() == "test.cmdi.xml" );
}

void sanityTest::test101b(){
  cout << " Metadata external reference (IMDI) ";
  Document doc = Document("file='tests/folia.imdi.xml'");
  CPPUNIT_ASSERT( doc.metadatatype() == IMDI );
  CPPUNIT_ASSERT( doc.metadatafile() == "test.imdi.xml" );
}

void sanityTest::test102( ){
  cout << " Add a word at wrong position ";
  FoliaElement *p = doc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  CPPUNIT_ASSERT_THROW( p->addWord("text='Ahoi'" ), ValueError );
}

void sanityTest::test102a(){
  cout << " Declarations - Default set ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( doc["example.text.1"]->select<Gap>()[0]->sett() == "gap-set" );
  CPPUNIT_ASSERT_THROW( doc.declare( AnnotationType::TOKEN, 
				     "some-set", 
				     "annotatorname='proycon'" ), XmlError );

}

void sanityTest::test102b(){
  cout << " Declarations - Set mismatching";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gip-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_THROW( doc.readFromString(xml), XmlError );
}

void sanityTest::test102c(){
  cout << " Declarations - Multiple sets for the same annotation type ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\" set=\"extended-gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( doc["example.text.1"]->select<Gap>()[0]->sett() == "gap-set" );
  CPPUNIT_ASSERT( doc["example.text.1"]->select<Gap>()[1]->sett() == "extended-gap-set" );
}

void sanityTest::test102d1(){
  cout << " Declarations - Multiple sets for the same annotation type (testing failure)";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_THROW( doc.readFromString(xml), XmlError );
}

void sanityTest::test102d2(){
  cout << " Declarations - Multiple sets for the same annotation type (testing failure)";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\" set=\"gip-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_THROW( doc.readFromString(xml), XmlError );
}

void sanityTest::test102d3(){
  cout << " Declarations - Ignore Duplicates";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.9\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( doc.defaultset(AnnotationType::GAP) == "gap-set" );
  CPPUNIT_ASSERT( doc.defaultannotator(AnnotationType::GAP,"gap-set") == "sloot" );
}

void sanityTest::test102e(){
  cout << " Declarations - Missing declaration ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"extended-gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_THROW( doc.readFromString(xml), XmlError );
}

void sanityTest::test102f(){
  cout << " Declarations - Declaration not needed ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap>\n"
"     <desc>test</desc>\n"
"    </gap>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );

}

void sanityTest::test102g(){
  cout << " Declarations - Empty set ";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\">\n"
"     <desc>test1</desc>\n"
"    </gap>\n"
"    <gap set=\"undefined\" class=\"Y\">\n"
"     <desc>test2</desc>\n"
"    </gap>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  CPPUNIT_ASSERT( v[0]->description() == "test1" );
  CPPUNIT_ASSERT( v[0]->sett() == "undefined" );
  CPPUNIT_ASSERT( v[1]->description() == "test2" );
  CPPUNIT_ASSERT( v[1]->sett() == "undefined" );
  CPPUNIT_ASSERT( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\"><desc>test2</desc></gap>" );
}

void sanityTest::test102h(){
  cout << " Declarations - Adding a declaration in same set. (other annotator)";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::GAP, 
					"gap-set", 
					"annotator='proycon'" ) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  CPPUNIT_ASSERT( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"sloot\" class=\"X\" set=\"gap-set\"/>" );
}

void sanityTest::test102i(){
  cout << " Declarations - miscellanious trouble";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap1-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap2-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap1-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( doc.defaultannotator(AnnotationType::GAP,"gap1-set") == "sloot" );
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::GAP, 
					"gap1-set", 
					"annotator='proycon'" ) );
  CPPUNIT_ASSERT( doc.defaultannotator(AnnotationType::GAP,"gap1-set") == "" );
  CPPUNIT_ASSERT( doc.defaultannotator(AnnotationType::GAP,"gap2-set") == "sloot" );
  FoliaElement *text = doc["example.text.1"];
  KWargs args = getArgs( "set='gap1-set', cls='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='gap1-set', cls='Z1'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='gap2-set', cls='Z2'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='gap2-set', cls='Y2', annotator='onbekend'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  CPPUNIT_ASSERT( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"sloot\" class=\"X\" set=\"gap1-set\"/>" );
  CPPUNIT_ASSERT( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"proycon\" class=\"Y\" set=\"gap1-set\"/>" );
  CPPUNIT_ASSERT( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z1\" set=\"gap1-set\"/>" );
  CPPUNIT_ASSERT( v[3]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z2\" set=\"gap2-set\"/>" );
  CPPUNIT_ASSERT( v[4]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"onbekend\" class=\"Y2\" set=\"gap2-set\"/>" );
}

void sanityTest::test102j(){
  cout << " Declarations - Adding a declaration in other set.";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  FoliaElement *text = doc["example.text.1"];
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::GAP, 
					"other-set", 
					"annotator='proycon'" ) );
  KWargs args = getArgs( "set='other-set', cls='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='other-set', cls='Z'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>();
  CPPUNIT_ASSERT( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\" set=\"gap-set\"/>" );
  CPPUNIT_ASSERT( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\" set=\"other-set\"/>" );
  CPPUNIT_ASSERT( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z\" set=\"other-set\"/>" );
}

void sanityTest::test102k(){
  cout << " Declarations - Several annotator types.";
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotatortype=\"auto\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;
  
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  FoliaElement *text = doc["example.text.1"];
  CPPUNIT_ASSERT( doc.defaultannotatortype(AnnotationType::GAP) == "auto" );
  vector<Gap*> v = text->select<Gap>();
  CPPUNIT_ASSERT( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\"/>" );
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::GAP, 
					"gap-set", 
					"annotatortype='manual'" ) );
  CPPUNIT_ASSERT( doc.defaultannotatortype(AnnotationType::GAP) == "" );
  KWargs args = getArgs( "set='gap-set', cls='Y', annotatortype='unknown'" );
  FoliaElement *g = 0;
  CPPUNIT_ASSERT_THROW( g = new Gap( &doc, args ), ValueError );
  args = getArgs( "set='gap-set', cls='Y', annotatortype='manual'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='gap-set', cls='Z', annotatortype='auto'" );
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &doc, args ) );
  text->append( g );
  v = text->select<Gap>();
  CPPUNIT_ASSERT( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"X\" set=\"gap-set\"/>" );
  CPPUNIT_ASSERT( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"manual\" class=\"Y\" set=\"gap-set\"/>" );
  CPPUNIT_ASSERT( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"Z\" set=\"gap-set\"/>" );
}

void sanityTest::test103( ){
  cout << " Alien namespaces - Checking whether properly ignored ";
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
    "xmlns=\"http://ilk.uvt.nl/folia\" xmlns:alien=\"http://somewhere.else\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations/>\n"
    "  </metadata>\n"
    "  <text xml:id=\"example.text.1\">\n"
    "    <s xml:id=\"example.text.1.s.1\">\n"
    "      <alien:blah >\n"
    "        <w xml:id=\"example.text.1.s.1.alienword\">\n"
    "        </w>\n"
    "      </alien:blah >\n"
    "      <w xml:id=\"example.text.1.s.1.w.1\">\n"
    "          <t>word</t>\n"
    "          <alien:invasion number=\"99999\" />\n"
    "      </w>\n"   
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n" ;
  Document doc;
  CPPUNIT_ASSERT_NO_THROW( doc.readFromString(xml) );
  CPPUNIT_ASSERT( len(doc.words()) == 1 ); // second word is in alien namespace
  // not read
  FoliaElement *w = doc["example.text.1.s.1.alienword"];
  CPPUNIT_ASSERT( w == 0 );   // doesn't exist
  w = doc["example.text.1.s.1.w.1"];
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>word</t></w>" );
}


class editTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( editTest );
  CPPUNIT_TEST( test001a );
  CPPUNIT_TEST( test001b );
  CPPUNIT_TEST( test002 );
  CPPUNIT_TEST( test003 );
  CPPUNIT_TEST( test003b );
  CPPUNIT_TEST( test004 );
  CPPUNIT_TEST( test005 );
  CPPUNIT_TEST( test006 );
  CPPUNIT_TEST( test007 );
  CPPUNIT_TEST( test008 );
  CPPUNIT_TEST( test009a );
  // CPPUNIT_TEST( test009b );
  // CPPUNIT_TEST( test009c );
  CPPUNIT_TEST( test010 );
  CPPUNIT_TEST( test011 );
  CPPUNIT_TEST( test012 );
  CPPUNIT_TEST( test013 );
  CPPUNIT_TEST( test014 );
  CPPUNIT_TEST( test015 );
  CPPUNIT_TEST( test016 );
  CPPUNIT_TEST( test017 );
  CPPUNIT_TEST( test018a );
  CPPUNIT_TEST( test018b );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
protected:
  void test001a();
  void test001b();
  void test002();
  void test003();
  void test003b();
  void test004();
  void test005();
  void test006();
  void test007();
  void test008();
  void test009a();
  // void test009b();
  // void test009c();
  void test010();
  void test011();
  void test012();
  void test013();
  void test014();
  void test015();
  void test016();
  void test017();
  void test018a();
  void test018b();
  Document doc;
};


void editTest::setUp( ){
  doc.readFromFile( "tests/folia.example" );
}

void editTest::test001a( ){
  cout << " Add a sentence to the last paragraph ";
  FoliaElement *p = 0;
  // grab last paragraph
  CPPUNIT_ASSERT_NO_THROW( p = doc.rparagraphs(0) );
  size_t tmp = len(p->sentences());
  // add a sentence
  FoliaElement *s = 0;
  CPPUNIT_ASSERT_NO_THROW( s = new Sentence( &doc, "generate_id='" + p->id() + "'" ) );
  p->append( s );
  // add words to the sentence
  Word *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='Dit', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='is', annotator='testscript', annotatortype='AUTO', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='een', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='nieuwe', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='zin', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "', space='no'" ) );
  s->append( w );  
  CPPUNIT_ASSERT_NO_THROW( w = new Word( &doc, "text='.', cls='PUNCTUATION', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  for ( int i=0; i < 6; ++i ){
    CPPUNIT_ASSERT( s->index(i)->id() == s->id() + ".w." + toString(i+1) );
  }

  // index check
  CPPUNIT_ASSERT( doc[s->id()] == s );
  CPPUNIT_ASSERT( doc[s->id() + ".w.3"] == s->index(2) );

  // attribute check
  CPPUNIT_ASSERT( s->index(0)->annotator() == "testscript" );
  CPPUNIT_ASSERT( s->index(2)->annotatortype() == AUTO );

  // adition to paragraph correct?
  CPPUNIT_ASSERT( p->size() == tmp+1 );
  CPPUNIT_ASSERT( p->rindex(0) == s );

  // last word ok?
  CPPUNIT_ASSERT( w->cls() == "PUNCTUATION" );
  CPPUNIT_ASSERT( w->text() == "." );

  // sentence ok?
  CPPUNIT_ASSERT( s->text() == "Dit is een nieuwe zin." );

  // all well?

  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\" annotator=\"testscript\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\" annotator=\"testscript\"><t>is</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\" annotator=\"testscript\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\" annotator=\"testscript\"><t>nieuwe</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\" annotator=\"testscript\" space=\"no\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" annotator=\"testscript\" class=\"PUNCTUATION\"><t>.</t></w></s>" );
}


void editTest::test001b( ){
  cout << " Add a sentence to the last paragraph (shortcuts)";
  FoliaElement *p = 0;
  // grab last paragraph
  CPPUNIT_ASSERT_NO_THROW( p = doc.rparagraphs(0) );
  size_t tmp = p->size();
  // add a sentence
  FoliaElement *s = 0;
  CPPUNIT_ASSERT_NO_THROW( s = p->addSentence() );
  // add words to the sentence
  KWargs ann;
  ann["text"] = "Dit";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( ann ) );
  CPPUNIT_ASSERT_NO_THROW( s->addWord( "text='is'" ) );
  ann["text"] = "een";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( ann ) );
  ann["text"] = "nieuwe";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( ann ) );
  ann["text"] = "zin";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = s->addWord( ann ) );
  FoliaElement *w2 = 0;
  CPPUNIT_ASSERT_NO_THROW( w2 = s->addWord( "text='.', cls='PUNCTUATION'" ) );
  
  CPPUNIT_ASSERT( len(s->words()) == 6 );
  CPPUNIT_ASSERT( w->text() == "zin" );
  CPPUNIT_ASSERT( doc[w->id()] == w );
  
  CPPUNIT_ASSERT( w2->text() == "." );

  // adition to paragraph correct?
  CPPUNIT_ASSERT( len( p->sentences() ) == tmp+1 );
  CPPUNIT_ASSERT( p->rindex(0) == s );
  
  // all well?
  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\"><t>is</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\"><t>nieuwe</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" class=\"PUNCTUATION\"><t>.</t></w></s>" );

}

void editTest::test002( ){
  cout << " Add an token annotation (pos, lemma) ";
  // grab a word (naam)
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );
  
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::POS, 
					"adhocpos") );

  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::LEMMA, 
					"adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_NO_THROW( w->addPosAnnotation( args ) );
  args = getArgs( "set='adhoclemma', cls='NAAM', annotator='testscript', annotatortype='auto', datetime='1982-12-15T19:00:01'");
  CPPUNIT_ASSERT_NO_THROW( w->addLemmaAnnotation( args ) );

  FoliaElement *p = 0;
  CPPUNIT_ASSERT_NO_THROW( p = w->annotation<PosAnnotation>( "adhocpos") );
  CPPUNIT_ASSERT( p->isinstance<PosAnnotation>() );
  CPPUNIT_ASSERT( p->cls() == "NOUN" );

  CPPUNIT_ASSERT_NO_THROW( p = w->annotation<LemmaAnnotation>( "adhoclemma") );
  CPPUNIT_ASSERT( p->isinstance<LemmaAnnotation>() );
  CPPUNIT_ASSERT( p->cls() == "NAAM" );
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" datetime=\"1982-12-15T19:00:01\" set=\"adhoclemma\"/></w>");
  
}

void editTest::test003( ){
  cout << " Add an token annotation (pos, lemma) (alternative) ";
  // grab a word (naam)
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::POS, "adhocpos") );

  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::LEMMA, "adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_NO_THROW( w->addAnnotation<PosAnnotation>( args ) );
  args = getArgs( "set='adhoclemma', cls='NAAM', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_NO_THROW( w->addAnnotation<LemmaAnnotation>( args ) );

  FoliaElement *p = 0;
  CPPUNIT_ASSERT_NO_THROW( p = w->annotation<PosAnnotation>( "adhocpos") );
  CPPUNIT_ASSERT( p->isinstance<PosAnnotation>() );
  CPPUNIT_ASSERT( p->cls() == "NOUN" );

  CPPUNIT_ASSERT_NO_THROW( p = w->annotation<LemmaAnnotation>( "adhoclemma") );
  CPPUNIT_ASSERT( p->isinstance<LemmaAnnotation>() );
  CPPUNIT_ASSERT( p->cls() == "NAAM" );

  // check the outcome
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" set=\"adhoclemma\"/></w>");
}

void editTest::test003b( ){
  cout << " Add an token annotation (pos, lemma) with missing declaration ";
  // grab a word (naam)
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::POS, "adhocpos") );

  // add a pos annotation (in a different set than the one declared,
  // to generate a conflict)
  KWargs args = getArgs( "set='adhopcos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_THROW( w->addAnnotation<PosAnnotation>( args ), ValueError );
}

void editTest::test004( ){
  cout << " Add a token default-set annotation which gives a name clash ";
  // grab a word (naam)
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );
  // add a pos annotation without specifying a set (should take default set), but this will clash with existing tag!
  KWargs args = getArgs( "cls='N', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_THROW( w->addPosAnnotation( args ), DuplicateAnnotationError );
  args = getArgs( "cls='naam', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_THROW( w->addLemmaAnnotation( args ), DuplicateAnnotationError );
}

void editTest::test005( ){
  cout << " Adding an alternative token annotation ";
  // grab a word (naam)
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"];
  KWargs args = getArgs( "cls='V'" );
  CPPUNIT_ASSERT_NO_THROW( w->addAlternative<PosAnnotation>( args ) );
  vector<Alternative*> alt = w->alternatives(); // all alternatives
  string sett = doc.defaultset(AnnotationType::POS);
  vector<Alternative*> alt2 = w->alternatives(sett);
  CPPUNIT_ASSERT( alt.size() == 1 );
  CPPUNIT_ASSERT( alt2.size() == 1 );
  CPPUNIT_ASSERT( alt[0] == alt2[0] );
  FoliaElement *p;
  CPPUNIT_ASSERT( p = w->annotation<PosAnnotation>( sett ) );
  CPPUNIT_ASSERT( p->isinstance<PosAnnotation>() );

  std::vector<Alternative *> alt3;
  CPPUNIT_ASSERT_NO_THROW( alt3 = w->alternatives(Pos_t, sett) );
  CPPUNIT_ASSERT( alt3.size() == 1 );
  CPPUNIT_ASSERT( alt[0] == alt3[0] );

  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"naam\"/><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt.1\" auth=\"no\"><pos class=\"V\"/></alt></w>" );

}

void editTest::test006( ){
  cout << " Correcting text ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // stippelijn
  CPPUNIT_ASSERT_NO_THROW( w->correct("new='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'"  ) );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>() );
  CPPUNIT_ASSERT( c->getOriginal()->index(0)->text() == "stippelijn" );
  CPPUNIT_ASSERT( c->getNew()->index(0)->text() == "stippellijn" );
  CPPUNIT_ASSERT( w->text() == "stippellijn" );

  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/></w>" );
 
}

void editTest::test007( ){
  cout << " Correcting Token Annotation ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // alweer stippelijn
  FoliaElement *oldpos = w->annotation<PosAnnotation>();
  FoliaElement *newpos = new PosAnnotation( &doc, "cls='N(soort,ev,basis,zijd,stan)'" );
  KWargs args = getArgs( "set='corrections', cls='spelling', annotator='testscript', annotatortype='auto'" );
  CPPUNIT_ASSERT_NO_THROW( w->correct( oldpos, newpos, args ) );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT( (c = w->annotation<Correction>()) != 0 );
  CPPUNIT_ASSERT( c->getOriginal()->index(0) == oldpos );
  CPPUNIT_ASSERT( (*c->getNew())[0] == newpos );

  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><pos class=\"N(soort,ev,basis,zijd,stan)\"/></new><original auth=\"no\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/></original></correction><lemma class=\"stippelijn\"/></w>" );
}

void editTest::test008( ){
  cout << " Suggesting a text correction ";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"]; // stippelijn
  CPPUNIT_ASSERT_NO_THROW( w->correct( "suggestion='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'" ) );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>() );
  CPPUNIT_ASSERT_NO_THROW( c = c->suggestions()[0] );
  CPPUNIT_ASSERT( c->text() == "stippellijn" );
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><suggestion auth=\"no\"><t>stippellijn</t></suggestion></correction></w>" );        
}

void editTest::test009a( ){
  cout << " Exception on duplicate ID ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  KWargs kw;
  kw["text"] = "stippellijn";
  kw["id"] = "WR-P-E-J-0000000001.p.1.s.8.w.11";
  CPPUNIT_ASSERT_THROW( w->sentence()->addWord( kw ),
			DuplicateIDError );
}

// void editTest::test009b( ){
//   cout << " Exception on adding TextContent of wrong level ";
//   FoliaElement *w = 0;
//   CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
//   CPPUNIT_ASSERT_THROW( w->settext( "bla", "original" ), ValueError );
// }

// void editTest::test009c( ){
//   cout << " Exception on adding duplicate TextContent ";
//   FoliaElement *w = 0;
//   CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
//   TextContent *t = new TextContent( "value='blah', corrected='yes'" );
//   CPPUNIT_ASSERT_THROW( w->append( t ), DuplicateAnnotationError );
// }

void editTest::test010( ){
  cout << " Creating an initially document-less tokenannotation element and adding it to a word ";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  FoliaElement *pos = 0;
  CPPUNIT_ASSERT_THROW( pos = new PosAnnotation( &doc, "set='fakecgn', cls='N'" ), ValueError );
  CPPUNIT_ASSERT_NO_THROW( doc.declare( AnnotationType::POS, 
					"fakecgn") );
  CPPUNIT_ASSERT_NO_THROW( pos = new PosAnnotation( &doc, "set='fakecgn', cls='N'" ) );
  CPPUNIT_ASSERT_NO_THROW( w->append( pos ) );
  CPPUNIT_ASSERT( pos == w->annotation<PosAnnotation>("fakecgn") );
  CPPUNIT_ASSERT( pos->parent() == w );
  CPPUNIT_ASSERT( pos->doc() == w->doc() );

  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"stippelijn\"/><pos class=\"N\" set=\"fakecgn\"/></w>" );
}

void editTest::test011(){
  cout << " Adding Subtoken annotation (morphological analysis)";
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.5.w.3"];
  FoliaElement *l = new MorphologyLayer(0);
  w->append( l );
  FoliaElement *m = new Morpheme( &doc );
  l->append( m );
  FoliaElement *t = new TextContent( &doc, "value='handschrift', offset='0'");
  m->append( t );
  FoliaElement *f = new Feature( &doc, "subset='type', cls='stem'");
  m->append( f );
  f = new Feature( &doc, "subset='function', cls='lexical'");
  m->append( f );
  m = new Morpheme( &doc );
  l->append( m );
  t = new TextContent( &doc, "value='en', offset='11'");
  m->append( t );
  f = new Feature( &doc, "subset='type', cls='suffix'");
  m->append( f );
  f = new Feature( &doc, "subset='function', cls='plural'" );
  m->append( f );
  CPPUNIT_ASSERT( len(l) ==  2 );  // 2 morphemes
  CPPUNIT_ASSERT( isinstance( l->index(0), Morpheme_t ) );
  CPPUNIT_ASSERT( l->index(0)->text() == "handschrift" );
  CPPUNIT_ASSERT( l->index(0)->feat("type") == "stem" );
  CPPUNIT_ASSERT( l->index(0)->feat("function") == "lexical" );
  CPPUNIT_ASSERT( l->index(1)->text() == "en" );
  CPPUNIT_ASSERT( l->index(1)->feat("type") == "suffix" );
  CPPUNIT_ASSERT( l->index(1)->feat("function") == "plural" );
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.5.w.3\"><t>handschriften</t><pos class=\"N(soort,mv,basis)\"/><lemma class=\"handschrift\"/><morphology><morpheme><t offset=\"0\">handschrift</t><feat class=\"stem\" subset=\"type\"/><feat class=\"lexical\" subset=\"function\"/></morpheme><morpheme><t offset=\"11\">en</t><feat class=\"suffix\" subset=\"type\"/><feat class=\"plural\" subset=\"function\"/></morpheme></morphology></w>" );
}

void editTest::test012(){
  cout << " Alignment ";
  throw NotImplementedError( "test012" );
}

void editTest::test013(){
  cout << " Adding Span Annotatation (syntax) ";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.4"];
  //sentence: 'De hoofdletter A wordt gebruikt voor het originele handschrift .'
  FoliaElement *layer = s->append( new SyntaxLayer(&doc) );
  FoliaElement *sent = layer->append( new SyntacticUnit( &doc, "cls='s'" ) );
  FoliaElement *np = sent->append( new SyntacticUnit( &doc,"cls='np'" ) );
  FoliaElement *su = np->append( new SyntacticUnit( &doc,"cls='det'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.1"] );
  su = np->append( new SyntacticUnit( &doc,"cls='n'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.2"] );
  su = np->append( new SyntacticUnit( &doc,"cls='n'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.3"] );
  FoliaElement *vp = sent->append( new SyntacticUnit( &doc,"cls='vp'" ) );
  FoliaElement *vps = vp->append( new SyntacticUnit(&doc, "cls='vp'" ) );
  su = vps->append( new SyntacticUnit( &doc,"cls='v'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.4"] );
  su = vps->append( new SyntacticUnit( &doc,"cls='participle'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.5"] );
  FoliaElement *pp = vp->append( new SyntacticUnit( &doc,"cls='pp'" ) );
  su = pp->append( new SyntacticUnit( &doc,"cls='prep'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.6"] );
  FoliaElement *nps = pp->append( new SyntacticUnit( &doc,"cls='np'" ) );
  su = nps->append( new SyntacticUnit( &doc,"cls='det'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.7"] );
  su = nps->append( new SyntacticUnit( &doc,"cls='adj'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.8"] );
  su = nps->append( new SyntacticUnit( &doc,"cls='n'" ) );
  su->append( doc["WR-P-E-J-0000000001.p.1.s.4.w.9"] );

  CPPUNIT_ASSERT( layer->xmlstring() == "<syntax xmlns=\"http://ilk.uvt.nl/folia\"><su class=\"s\"><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.1\" t=\"De\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.2\" t=\"hoofdletter\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.3\" t=\"A\"/></su></su><su class=\"vp\"><su class=\"vp\"><su class=\"v\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.4\" t=\"wordt\"/></su><su class=\"participle\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.5\" t=\"gebruikt\"/></su></su><su class=\"pp\"><su class=\"prep\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.6\" t=\"voor\"/></su><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.7\" t=\"het\"/></su><su class=\"adj\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.8\" t=\"originele\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.9\" t=\"handschrift\"/></su></su></su></su></su></syntax>" );
}

void editTest::test014() {
  cout << " Replacing an annotation " ;
  FoliaElement *word = doc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->replace( new PosAnnotation( &doc, "cls='BOGUS'") );
  CPPUNIT_ASSERT( len(word->annotations<PosAnnotation>() ) ==  1 );
  CPPUNIT_ASSERT(  word->annotation<PosAnnotation>()->cls() == "BOGUS" );
  CPPUNIT_ASSERT( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/><pos class=\"BOGUS\"/></w>" );
}

void editTest::test015(){
  cout << " Removing an annotation ";
  FoliaElement *word = doc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->remove( word->annotation<PosAnnotation>() );
  CPPUNIT_ASSERT_THROW( word->annotations<PosAnnotation>(), NoSuchAnnotation );
  CPPUNIT_ASSERT( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/></w>" );
}

void editTest::test016(){
  cout << " Time Stamp ";
  FoliaElement *word = doc["WR-P-E-J-0000000001.p.1.s.8.w.16"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  CPPUNIT_ASSERT_NO_THROW( pos->setDateTime( "1982-12-15T19:00:01" ) );

  CPPUNIT_ASSERT( pos->xmlstring() == "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"WW(pv,tgw,met-t)\" datetime=\"1982-12-15T19:00:01\"/>" );      
}

void editTest::test017(){
  cout << " Altering word text";
  // Important note: directly altering text is usually bad practise, you'll want to use proper corrections instead.
  FoliaElement *word = doc["WR-P-E-J-0000000001.p.1.s.8.w.9"];
  CPPUNIT_ASSERT( word->text() == "terweil" );
  word->settext("terwijl");
  CPPUNIT_ASSERT( word->text() == "terwijl" );
  CPPUNIT_ASSERT( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terwijl</t><errordetection class=\"spelling\" error=\"yes\"/><pos class=\"VG(onder)\"/><lemma class=\"terweil\"/></w>" );      
}

void editTest::test018a(){
  cout << " Altering sentence text (untokenized by definition)";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.1"];
  CPPUNIT_ASSERT( !s->hastext() ); // No text directly associated
  // but text() can be retrieved from children.
  CPPUNIT_ASSERT( s->text() == "Stemma is een ander woord voor stamboom ." );

  s->settext( "Hooked on text." );
  CPPUNIT_ASSERT( s->hastext() ); // Now there IS direct text
  CPPUNIT_ASSERT( s->text() == "Hooked on text." );
}

void editTest::test018b(){
  cout << " Altering sentence text (untokenized by definition)";
  FoliaElement *s = doc["WR-P-E-J-0000000001.p.1.s.8"];
  CPPUNIT_ASSERT( s->text() == "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  CPPUNIT_ASSERT_NO_THROW( s->settext("Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt." ) );
  CPPUNIT_ASSERT_NO_THROW( s->settext("Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.", "original" ) );
  CPPUNIT_ASSERT( s->text() == "Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt." );
  CPPUNIT_ASSERT( s->hastext("original") );
  CPPUNIT_ASSERT( s->text("original") == "Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt." );

  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8\"><t>Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt.</t><t class=\"original\">Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.</t><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.1\"><t>Een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><quote xml:id=\"WR-P-E-J-0000000001.p.1.s.8.q.1\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.2\"><t>volle</t><pos class=\"ADJ(prenom,basis,met-e,stan)\"/><lemma class=\"vol\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.3\"><t>lijn</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"lijn\"/></w></quote><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.4\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.5\"><t>op</t><pos class=\"VZ(init)\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.6\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.7\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.8\"><t>,</t><pos class=\"LET()\"/><lemma class=\",\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terweil</t><errordetection class=\"spelling\" error=\"yes\"/><pos class=\"VG(onder)\"/><lemma class=\"terweil\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.10\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.12\"><t>op</t><pos class=\"VZ(init)\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.13\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14\"><t>onzekere</t><pos class=\"ADJ(prenom,basis,met-e,stan)\"/><lemma class=\"onzeker\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14.c.1\" class=\"spelling\"><suggestion auth=\"no\"><t>twijfelachtige</t></suggestion><suggestion auth=\"no\"><t>ongewisse</t></suggestion></correction></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.15\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.16\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.17\"><t>.</t><pos class=\"LET()\"/><lemma class=\".\"/></w></s>" );
}

class createTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( createTest );
  CPPUNIT_TEST( test001 );
  CPPUNIT_TEST( test002 );
  CPPUNIT_TEST( test003 );
  CPPUNIT_TEST_SUITE_END();
public:
protected:
  void test001();
  void test002();
  void test003();
  Document doc;
};


void createTest::test001( ){
  cout << " Creating a document from scratch. ";
  Document d( "id='example'" );
  CPPUNIT_ASSERT_NO_THROW( d.declare( AnnotationType::TOKEN, 
				      "adhocset", 
				      "annotator='proycon'" ) );
  CPPUNIT_ASSERT( d.defaultset(AnnotationType::TOKEN) == "adhocset" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::TOKEN) == "proycon" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  CPPUNIT_ASSERT_NO_THROW( text = d.addNode( Text_t, kw ) );
  kw.clear();
  FoliaElement *s = 0;
  CPPUNIT_ASSERT_NO_THROW( s = new Sentence( &d, "generate_id='" + text->id() + "'" ) );
  text->append( s );
  kw.clear();
  kw["text"] = "De";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( kw ) );
  kw["text"] = "site";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( kw ) );
  kw["text"] = "staat";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( kw ) );
  kw["text"] = "online";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( kw ) );
  kw["text"] = ".";
  CPPUNIT_ASSERT_NO_THROW( s->addWord( kw ) );
  //  CPPUNIT_ASSERT_NO_THROW( d.save( "/tmp/foliacreatetest001.xml" ) );
  CPPUNIT_ASSERT( d[id+".s.1"]->size() == 5 );
}

void createTest::test002( ){
  cout << " Creating a document from scratch. ";
  Document d( "id='example'" );
  CPPUNIT_ASSERT_NO_THROW( d.declare( AnnotationType::POS, 
				      "adhocset", 
				      "annotator='proycon'" ) );
  CPPUNIT_ASSERT_NO_THROW( d.declare( AnnotationType::POS, 
				      "myset", 
				      "annotator='sloot'" ) );
  CPPUNIT_ASSERT( d.defaultset(AnnotationType::POS) == "" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::POS) == "" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::POS, "myset") == "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  CPPUNIT_ASSERT_NO_THROW( text = d.addNode( Text_t, kw ) );
  kw.clear();
  FoliaElement *s = 0;
  CPPUNIT_ASSERT_NO_THROW( s = new Sentence( &d, "generate_id='" + text->id() + "'" ) );
  text->append( s );
  kw.clear();
  kw["text"] = "landen";
  FoliaElement *w = 0;
  CPPUNIT_ASSERT_NO_THROW( w = s->addWord( kw ) );
  kw.clear();
  kw["set"] = "myset";
  kw["cls"] = "NP";
  CPPUNIT_ASSERT_NO_THROW( w->addAnnotation<PosAnnotation>( kw ) );
  kw["cls"] = "VP";
  kw["set"] = "adhocset";
  CPPUNIT_ASSERT_NO_THROW( w->addAnnotation<PosAnnotation>( kw ) );
  vector<PosAnnotation*> v = w->select<PosAnnotation>( "adhocset" );
  CPPUNIT_ASSERT( v.size() == 1 );
  vector<PosAnnotation*> v1 = w->select<PosAnnotation>( "myset" );
  CPPUNIT_ASSERT( v1.size() == 1 );
  vector<PosAnnotation*> v2 = w->select<PosAnnotation>( "noset" );
  CPPUNIT_ASSERT( v2.size() == 0 ); 
  vector<PosAnnotation*> v3 = w->select<PosAnnotation>();
  CPPUNIT_ASSERT( v3.size() == 2 ); 
  CPPUNIT_ASSERT_NO_THROW( d.save( "/tmp/foliacreatetest002.xml" ) );
  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>landen</t><pos class=\"NP\" set=\"myset\"/><pos class=\"VP\" set=\"adhocset\"/></w>" );
}

void createTest::test003( ){
  cout << " Creating a document with gap annotations from scratch. ";
  Document d( "id='example'" );
  CPPUNIT_ASSERT_NO_THROW( d.declare( AnnotationType::GAP, 
				      "gap-set",
				      "annotator='sloot'" ) );
  CPPUNIT_ASSERT_NO_THROW( d.declare( AnnotationType::GAP, 
				      "extended-gap-set", 
				      "annotator='sloot'" ) );
  CPPUNIT_ASSERT( d.defaultset(AnnotationType::GAP) == "" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::GAP) == "" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::GAP, "gap-set") == "sloot" );
  CPPUNIT_ASSERT( d.defaultannotator(AnnotationType::GAP, "extended-gap-set") == "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  CPPUNIT_ASSERT_NO_THROW( text = d.addNode( Text_t, kw ) );
  kw.clear();
  kw["set"] = "gap-set";
  kw["cls"] = "NP";
  FoliaElement *g = 0;
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &d, kw ) );
  text->append( g );
  kw.clear();
  kw["set"] = "extended-gap-set";
  kw["cls"] = "VP";
  CPPUNIT_ASSERT_NO_THROW( g = new Gap( &d, kw ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>( "gap-set" );
  CPPUNIT_ASSERT( v.size() == 1 );
  vector<Gap*> v1 = text->select<Gap>( "extended-gap-set" );
  CPPUNIT_ASSERT( v1.size() == 1 );
  vector<Gap*> v3 = text->select<Gap>();
  CPPUNIT_ASSERT( v3.size() == 2 ); 
  //  CPPUNIT_ASSERT_NO_THROW( d.save( "/tmp/foliacreatetest003.xml" ) );
  //  cerr << "\n" << text->xmlstring() << endl;
  CPPUNIT_ASSERT( text->xmlstring() == "<text xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1\"><gap class=\"NP\" set=\"gap-set\"/><gap class=\"VP\" set=\"extended-gap-set\"/></text>" );
}

class correctionTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( correctionTest );
  CPPUNIT_TEST( test001a );
  CPPUNIT_TEST( test001b );
  CPPUNIT_TEST( test002 );
  CPPUNIT_TEST( test003 );
  CPPUNIT_TEST( test004 );
  CPPUNIT_TEST( test005 );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
protected:
  void test001a();
  void test001b();
  void test002();
  void test003();
  void test004();
  void test005();
  Document *doc;
  Text *text;
};

void correctionTest::setUp( ){
  doc = new Document( "id='example'" );
  CPPUNIT_ASSERT_NO_THROW( doc->declare( AnnotationType::TOKEN, 
					 "adhocset", 
					 "annotator='proycon'" ) );
  text = new Text( "id='" + doc->id() + ".text.1'" );
  doc->append( text );
}

void correctionTest::tearDown( ){
  delete doc;
}

void correctionTest::test001a( ){
  cout << " Split correction ";
  FoliaElement *s = text->append( new Sentence( "id='" + doc->id() + ".s.1'"  ) );
  s->append( new Word( doc, "text='De', id='" + doc->id() + ".s.1.w.1'" ) );
  s->append( new Word( doc, "text='site', id='" + doc->id() + ".s.1.w.2'" ) );
  s->append( new Word( doc, "text='staat', id='" + doc->id() + ".s.1.w.3'" ) );
  s->append( new Word( doc, "text='online', id='" + doc->id() + ".s.1.w.4'" ) );
  s->append( new Word( doc, "text='.', id='" + doc->id() + ".s.1.w.5'" ) );
  FoliaElement *w = doc->index(doc->id() + ".s.1.w.4");
  w->split( new Word( doc, "id='" + doc->id() + ".s.1.w.4a', text='on'" ),
   	    new Word( doc, "id='" + doc->id() + ".s.1.w.4b', text='line'" ) );
  //  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliasplit1a.xml" ) );
  s = doc->index("example.s.1");
  CPPUNIT_ASSERT( s->rwords(2)->text() == "on" );
  CPPUNIT_ASSERT( s->rwords(1)->text() == "line" );
  CPPUNIT_ASSERT( s->text() == "De site staat on line ." );
  CPPUNIT_ASSERT( len( s->words() ) == 6 );
  CPPUNIT_ASSERT( s->xmlstring() ==  "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4a\"><t>on</t></w><w xml:id=\"example.s.1.w.4b\"><t>line</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>online</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correctionTest::test001b( ){
  cout << " Split suggestion ";
  FoliaElement *s = text->append( new Sentence( "id='" + doc->id() + ".s.1'"  ) );
  s->append( new Word( doc, "text='De', id='" + doc->id() + ".s.1.w.1'" ) );
  s->append( new Word( doc, "text='site', id='" + doc->id() + ".s.1.w.2'" ) );
  s->append( new Word( doc, "text='staat', id='" + doc->id() + ".s.1.w.3'" ) );
  s->append( new Word( doc, "text='online', id='" + doc->id() + ".s.1.w.4'" ) );
  s->append( new Word( doc, "text='.', id='" + doc->id() + ".s.1.w.5'" ) );
  FoliaElement *w = doc->index(doc->id() + ".s.1.w.4");
  Word *w1 = new Word( doc, "generate_id='" + s->id() + "',text='on'" );
  Word *w2 = new Word( doc, "generate_id='" + s->id() + "',text='line'" );
  w->split( w1, w2, "suggest='true'" );
  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliasplit1b.xml" ) );
  s = doc->index("example.s.1");
  CPPUNIT_ASSERT( len( s->words() ) == 5 );
  CPPUNIT_ASSERT( len( doc->words() ) == 5 );
  CPPUNIT_ASSERT( s->rwords(1)->text() == "online" );
  CPPUNIT_ASSERT( s->text() == "De site staat online ." );
  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><current><w xml:id=\"example.s.1.w.4\"><t>online</t></w></current><suggestion auth=\"no\"><w xml:id=\"example.s.1.w.6\"><t>on</t></w><w xml:id=\"example.s.1.w.7\"><t>line</t></w></suggestion></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>");
}
     
void correctionTest::test002(){
  cout << " Merge corrections ";
  FoliaElement *s = text->append( new Sentence( "id='" + doc->id() + ".s.1'"	 ) );
  s->append( new Word( doc, "text='De', id='" + doc->id() + ".s.1.w.1'" ) );
  s->append( new Word( doc, "text='site', id='" + doc->id() + ".s.1.w.2'" ) );
  s->append( new Word( doc, "text='staat', id='" + doc->id() + ".s.1.w.3'" ) );
  s->append( new Word( doc, "text='on', id='" + doc->id() + ".s.1.w.4'" ) );
  s->append( new Word( doc, "text='line', id='" + doc->id() + ".s.1.w.5'" ) );
  s->append( new Word( doc, "text='.', id='" + doc->id() + ".s.1.w.6'" ) );

  vector<FoliaElement *> ow;
  ow.push_back( doc->index(doc->id() + ".s.1.w.4") );
  ow.push_back( doc->index(doc->id() + ".s.1.w.5") );
  s->mergewords( new Word( doc, "id='" + doc->id() + ".s.1.w.4-5', text='online'" ), ow );
  //  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliamerge002.xml" ) );
  CPPUNIT_ASSERT( len(s->words() ) ==  5 );
  CPPUNIT_ASSERT( s->text() == "De site staat online ." );
  // incorrection() test, check if newly added word correctly reports being part of a correction
  FoliaElement *w = doc->index(doc->id() + ".s.1.w.4-5");
  CPPUNIT_ASSERT( isinstance(w->incorrection(), Correction_t) );
  //incorrection return the correction the word is part of, or None if not part of a correction, 
  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4-5\"><t>online</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>on</t></w><w xml:id=\"example.s.1.w.5\"><t>line</t></w></original></correction><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>" );
}

void correctionTest::test003(){
  cout << " Delete corrections ";
  FoliaElement *s = text->append( new Sentence( "id='" + doc->id() + ".s.1'"	 ) );
  s->append( new Word( doc, "text='Ik', id='" + doc->id() + ".s.1.w.1'" ) );
  s->append( new Word( doc, "text='zie', id='" + doc->id() + ".s.1.w.2'" ) );
  s->append( new Word( doc, "text='een', id='" + doc->id() + ".s.1.w.3'" ) );
  s->append( new Word( doc, "text='groot', id='" + doc->id() + ".s.1.w.4'" ) );
  s->append( new Word( doc, "text='huis', id='" + doc->id() + ".s.1.w.5'" ) );
  s->append( new Word( doc, "text='.', id='" + doc->id() + ".s.1.w.6'" ) );

  s->deleteword( doc->index( doc->id() + ".s.1.w.4" ) );
  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliadelete003.xml" ) );
  CPPUNIT_ASSERT( s->text() == "Ik zie een huis ." );
  CPPUNIT_ASSERT( s->xmlstring() =="<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>groot</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>huis</t></w><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>");
}

void correctionTest::test004(){
  cout << " Insert corrections ";
  FoliaElement *s = text->append( new Sentence( "id='" + doc->id() + ".s.1'"	 ) );
  s->append( new Word( doc, "text='Ik', id='" + doc->id() + ".s.1.w.1'" ) );
  s->append( new Word( doc, "text='zie', id='" + doc->id() + ".s.1.w.2'" ) );
  s->append( new Word( doc, "text='een', id='" + doc->id() + ".s.1.w.3'" ) );
  s->append( new Word( doc, "text='huis', id='" + doc->id() + ".s.1.w.4'" ) );
  s->append( new Word( doc, "text='.', id='" + doc->id() + ".s.1.w.5'" ) );

  s->insertword( new Word( doc, "id='" + doc->id() + ".s.1.w.3b', text='groot'" ), doc->index( doc->id() + ".s.1.w.3" ) );
  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliainsert004.xml" ) );
  CPPUNIT_ASSERT( s->text() == "Ik zie een groot huis ." );
  CPPUNIT_ASSERT( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.3b\"><t>groot</t></w></new><original auth=\"no\"/></correction><w xml:id=\"example.s.1.w.4\"><t>huis</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correctionTest::test005(){
  cout << " Re-using a correction with only suggestions ";
  delete doc; // get rid of the setUp variant…
  doc = new Document();
  doc->readFromFile( "tests/folia.example" );
  FoliaElement *w = doc->index("WR-P-E-J-0000000001.p.1.s.8.w.11"); // stippelijn
  CPPUNIT_ASSERT_NO_THROW( w->correct("suggestion='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'" ) );
  CPPUNIT_ASSERT_NO_THROW( doc->save( "/tmp/foliainsert005-1.xml" ) );
  FoliaElement *c = 0;
  CPPUNIT_ASSERT_NO_THROW( c = w->annotation<Correction>() );
  CPPUNIT_ASSERT( c->suggestions()[0]->text() == "stippellijn" );
  CPPUNIT_ASSERT( w->text() == "stippelijn" );  

  CPPUNIT_ASSERT_NO_THROW( w->correct("new='stippellijn', set='corrections', cls='spelling',annotator='John Doe', annotatortype='manual', reuse='" + c->id() + "'" ) );

  CPPUNIT_ASSERT( w->text() == "stippellijn" );
  CPPUNIT_ASSERT( len(w->annotations<Correction>()) == 1 );
  CPPUNIT_ASSERT( w->annotation<Correction>()->suggestions()[0]->text() == "stippellijn" );
  CPPUNIT_ASSERT( w->annotation<Correction>()->suggestions()[0]->annotator() == "testscript" );
  CPPUNIT_ASSERT( w->annotation<Correction>()->suggestions()[0]->annotatortype() == AUTO );
  CPPUNIT_ASSERT( w->annotation<Correction>()->getNew()->text() == "stippellijn" );
  CPPUNIT_ASSERT( w->annotation<Correction>()->annotator() == "John Doe" );
  CPPUNIT_ASSERT( w->annotation<Correction>()->annotatortype() == MANUAL );

  CPPUNIT_ASSERT( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"John Doe\" class=\"spelling\"><suggestion annotator=\"testscript\" annotatortype=\"auto\" auth=\"no\"><t>stippellijn</t></suggestion><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction></w>" );
}

class queryTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( queryTest );
  CPPUNIT_TEST( test001 );
  CPPUNIT_TEST( test002 );
  CPPUNIT_TEST( test003 );
  CPPUNIT_TEST( test004 );
  CPPUNIT_TEST( test005 );
  CPPUNIT_TEST( test006 );
  CPPUNIT_TEST( test008 );
  CPPUNIT_TEST( test009 );
  CPPUNIT_TEST( test010a );
  CPPUNIT_TEST( test010b );
  CPPUNIT_TEST( test011 );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
protected:
  void test001();
  void test002();
  void test003();
  void test004();
  void test005();
  void test006();
  void test007();
  void test008();
  void test009();
  void test010a();
  void test010b();
  void test011();
  Document doc;
  Text *text;
};

void queryTest::setUp( ){
  doc.readFromFile( "tests/folia.example" );
}

void queryTest::test001(){
  cout << " Find Words (simple) ";
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 3 );
  CPPUNIT_ASSERT( matches[0][0]->text() == "van" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "het" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "alfabet" );
}

void queryTest::test002(){
  cout << " Find Words (with wildcard) ";
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "*:1" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 3 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "van" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "het" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "alfabet" );
}

void queryTest::test003(){
  cout << " Find Words by annotation ";
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "historisch" );
  words.push_back( "wetenschap" );
  words.push_back( "worden" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words,
							 Lemma_t ) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 4 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "historische" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "wetenschap" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "wordt" );
}

void queryTest::test004(){
  cout << " Find Words using multiple patterns ";
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "historische" );
  words.push_back( "*" );
  words.push_back( "wordt" );
  Pattern p1(words);
  words.clear();
  words.push_back( "de" );
  words.push_back( "historisch" );
  words.push_back( "wetenschap" );
  words.push_back( "worden" );
  Pattern p2(words,Lemma_t );
  list<Pattern> l;
  l.push_back( p1 );
  l.push_back( p2 );
  vector<vector<Word*> >matches = doc.findwords( l ); 
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 4 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "historische" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "wetenschap" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "wordt" );
}

void queryTest::test005(){
  cout << " Find Words that aren't there ";
  vector<string> words;
  words.push_back( "bli" );
  words.push_back( "bla" );
  words.push_back( "blu" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 0 );
}

void queryTest::test006(){
  cout << " Find words with overlap ";
  Document doc( "id='test'" );
  FoliaElement *text = new Text( "id='test.text'" );
  doc.append( text );
  FoliaElement *s = new Sentence( "id='" + doc.id() + ".s.1'" );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='A'" ); // test caseinsensitivness too
  s->addWord( "text='b'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  vector<string> words;
  words.push_back( "a" );
  words.push_back( "a" );  
  vector<vector<Word*> > matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 4 );
  matches = doc.findwords( Pattern(words,"casesensitive='1'") );
  CPPUNIT_ASSERT( matches.size() == 3 );
}

void queryTest::test007(){
  cout << " Find Words with context ";
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words),
						  "leftcontext='3', rightcontext='3'"); 
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 9 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "laatste" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "letters" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "van" );
  CPPUNIT_ASSERT( matches[0][4]->text() == "het" );
  CPPUNIT_ASSERT( matches[0][5]->text() == "alfabet" );
  CPPUNIT_ASSERT( matches[0][6]->text() == "en" );
  CPPUNIT_ASSERT( matches[0][7]->text() == "worden" );
  CPPUNIT_ASSERT( matches[0][8]->text() == "tussen" );
}

void queryTest::test008(){
  cout << " Find Words with 'disjunction' (by using regexp) ";
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "regexp('historische|hedendaagse')" );
  words.push_back( "wetenschap" );
  words.push_back( "wordt" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 4 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "historische" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "wetenschap" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "wordt" );
}

void queryTest::test009(){
  cout << " Find Words with regular expressions ";
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "regexp('hist.*')" );
  words.push_back( "regexp('.*schap')" );
  words.push_back( "regexp('w[oae]rdt')" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 4 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "historische" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "wetenschap" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "wordt" );
}

void queryTest::test010a(){
  cout << " Find Words with variable wildcard ";
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "laatste" );
  words.push_back( "*" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 1 );
  CPPUNIT_ASSERT( len(matches[0]) == 6 );

  CPPUNIT_ASSERT( matches[0][0]->text() == "de" );
  CPPUNIT_ASSERT( matches[0][1]->text() == "laatste" );
  CPPUNIT_ASSERT( matches[0][2]->text() == "letters" );
  CPPUNIT_ASSERT( matches[0][3]->text() == "van" );
  CPPUNIT_ASSERT( matches[0][4]->text() == "het" );
  CPPUNIT_ASSERT( matches[0][5]->text() == "alfabet" );
}

void queryTest::test010b(){
  cout << " Find words with wildcard and overlap ";
  Document doc( "id='test'" );
  FoliaElement *text = new Text( "id='test.text'" );
  doc.append( text );
  FoliaElement *s = new Sentence( "id='" + doc.id() + ".s.1'" );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  s->addWord( "text='d'" );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  vector<string> words;
  words.push_back( "a" );
  words.push_back( "*" );  
  words.push_back( "c" );  
  vector<vector<Word*> > matches = doc.findwords( Pattern(words) );
  CPPUNIT_ASSERT( matches.size() == 3 );
}

void queryTest::test011(){
  cout << " Find Words by non existing annotation ";
  vector<string> words;
  words.push_back( "bla" );
  words.push_back( "bla" );
  words.push_back( "blu" );
  vector<vector<Word*> >matches = doc.findwords( Pattern(words,
							 Sense_t ) );
  CPPUNIT_ASSERT( matches.size() == 0 );
}


CPPUNIT_TEST_SUITE_REGISTRATION (foliaTest);
CPPUNIT_TEST_SUITE_REGISTRATION (sanityTest);
CPPUNIT_TEST_SUITE_REGISTRATION (editTest);
CPPUNIT_TEST_SUITE_REGISTRATION (createTest);
CPPUNIT_TEST_SUITE_REGISTRATION (correctionTest);
CPPUNIT_TEST_SUITE_REGISTRATION (queryTest);

int main(){
  CppUnit:: TestResult testresult;
  
  // register listener for collecting the test-results
  CppUnit::TestResultCollector collectedresults;
  testresult.addListener (&collectedresults);
  
  // register listener for per-test progress output
  CppUnit::BriefTestProgressListener progress;
  testresult.addListener (&progress);
  
  // insert test-suite at test-runner by registry
  CppUnit::TestRunner runner;
  runner.addTest (CppUnit::TestFactoryRegistry::getRegistry ().makeTest ());
  runner.run (testresult);
  
  // output results in compiler-format
  CppUnit:: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
  compileroutputter.write ();
  
  // return 0 if tests were successful
  return collectedresults.wasSuccessful () ? 0 : 1;
}
