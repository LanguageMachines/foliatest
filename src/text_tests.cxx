/*
  Copyright (c) 2006 - 2020
  CLST  - Radboud University
  ILK   - Tilburg University

  This file is part of foliatest

  foliatest is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  foliatest is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      https://github.com/LanguageMachines/foliatest/issues
  or send mail to:
      lamasoftware (at ) science.ru.nl

*/

#include <iostream>
#include <string>
#include "ticcutils/StringOps.h"
#include "ticcutils/PrettyPrint.h"
#include "libfolia/folia.h"
#include "ticcutils/UnitTest.h"

#include "config.h"
using namespace std;
using namespace icu;
using namespace folia;

using TiCC::operator<<;

void text_test01(){
  startTestSerie( " a text with miscellaneous spaces " );
  Document doc;
  assertNoThrow( doc.read_from_file("tests/spaces.xml") );
  if ( !hasThrown() ){
    FoliaElement *el = doc["spaces.text.p.1.s.1.w.1"];
    assertFalse( el->str() == "Zit ting" );
    assertEqual( el->str(), "Zit ting" );
    el = doc["spaces.text.p.3.s.1.w.2"];
    assertFalse( el->str() == "op die" );
    assertEqual( el->str(), "op die" );
    el = doc["spaces.text.p.3.s.1.w.4"];
    assertFalse( el->str() == "van de" );
    assertEqual( el->str(), "van de" );
    el = doc["spaces.text.p.3.s.1.w.5"];
    assertFalse( el->str() == "de ese" );
    assertEqual( el->str(), "de ese" );
    el = doc["spaces.text.p.3.s.1.w.6"];
    assertFalse( el->str() == "zij de" );
    assertEqual( el->str(), "zij de" );
  }
}

void text_test03(){
  startTestSerie( " Invalid Text (Misspelled word) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literrair oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.read_from_string(xml), InconsistentText );
}


void text_test04(){
  startTestSerie( " Invalid Text (Missing word) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.read_from_string(xml), InconsistentText );
}


void text_test05(){
  startTestSerie( " Valid Text (Intermittent Redundancy) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg? Of kan ik het ook?</t>"
"    <p xml:id=\"example.p.1\">"
"      <!-- Note: no text here on paragraph level -->"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"      <s xml:id=\"example.p.1.s.2\">"
"        <t>Of kan ik"
"het    ook   ?"
"            </t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.read_from_string(xml), InconsistentText );
}

void text_test06(){
  startTestSerie( " Valid Text (Multiple classes) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <t class=\"missingword\">Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literrair oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"        <t class=\"missingword\">Is het creëren van een vol<t-hbr/>waardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of<br/>Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.read_from_string(xml), InconsistentText );
}

void text_test07(){
  startTestSerie( "Validation - No text checking on (nested) morphemes" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <pos-annotation set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\" annotator=\"frog\" annotatortype=\"auto\" />"
"      <pos-annotation annotator=\"frog-mbma-1.0\" annotatortype=\"auto\" datetime=\"2017-04-20T16:48:45\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"      <lemma-annotation set=\"lemmas-nl\" annotator=\"tadpole\" annotatortype=\"auto\" />"
"      <morphological-annotation annotator=\"proycon\" annotatortype=\"manual\" />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"      <w xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.16\">"
"        <t>genealogie</t>"
"        <pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>"
"        <lemma class=\"genealogie\"/>"
"        <morphology>"
"          <morpheme class=\"complex\">"
"            <t>genealogie</t>"
"            <feat class=\"[[genealogisch]adjective[ie]]noun/singular\" subset=\"structure\"/>"
"            <pos class=\"N\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"            <morpheme class=\"complex\">"
"              <feat class=\"N_A*\" subset=\"applied_rule\"/>"
"              <feat class=\"[[genealogisch]adjective[ie]]noun\" subset=\"structure\"/>"
"              <pos class=\"N\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"              <morpheme class=\"stem\">"
"                <t>genealogisch</t>"
"                <pos class=\"A\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"              </morpheme>"
"              <morpheme class=\"affix\">"
"                <t>ie</t>"
"                <feat class=\"[ie]\" subset=\"structure\"/>"
"              </morpheme>"
"             </morpheme>"
"             <morpheme class=\"inflection\">"
"              <feat class=\"singular\" subset=\"inflection\"/>"
"             </morpheme>"
"          </morpheme>"
"        </morphology>"
"      </w>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
}

void text_test08(){
  startTestSerie( "Text - Offset validation" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"78\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"82\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"90\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"92\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"98\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"100\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"103\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"111\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
}

void text_test08b(){
  startTestSerie( "Text - Offset validation (legacy)" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
  assertEqual( doc.get_warn_count(), 8 );
}

void text_test09(){
  startTestSerie( "Text - Offset validation (invalid)" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"10\">van</t> <!-- this one is invalid -->"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"24\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.read_from_string(xml), UnresolvableTextContent );
}

void text_test10(){
  startTestSerie( "Text - Offset validation with explicit references" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\" ref=\"example.p.1\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\" ref=\"example.p.1\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\" ref=\"example.p.1\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\" ref=\"example.p.1\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\" ref=\"example.p.1\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\" ref=\"example.p.1\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\" ref=\"example.p.1\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\" ref=\"example.p.1\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\" ref=\"example.p.1\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\" ref=\"example.p.1\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\" ref=\"example.p.1\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"78\" ref=\"example.p.1\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"82\" ref=\"example.p.1\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"90\" ref=\"example.p.1\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"92\" ref=\"example.p.1\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"98\" ref=\"example.p.1\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"100\" ref=\"example.p.1\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"103\" ref=\"example.p.1\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"111\" ref=\"example.p.1\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
#if FOLIA_INT_VERSION >= 215
  int off = 0;
  assertEqual( doc["example.p.1.s.1.w.19"]->text_content()->get_reference(off),
	       doc["example.p.1"] ); // testing resolving explicit reference
#else
  assertEqual( doc["example.p.1.s.1.w.19"]->text_content()->get_reference(),
	       doc["example.p.1"] ); // testing resolving explicit reference
#endif
}

void text_test11(){
  startTestSerie( "Text - Offset validation with text markup (with text modifiers like br" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een <t-style class=\"emphasis\">volwaardig</t-style> literair oeuvre voorbehouden aan schrijvers<br/>\tals <t-str xlink:href=\"https://nl.wikipedia.org/wiki/Louis_Couperus\" xlink:type=\"simple\">Couperus</t-str>, 	Haasse, of \tGrunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"82\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"90\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"92\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"98\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"100\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"103\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"111\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
#if FOLIA_INT_VERSION >= 215
  int off = 0;
  assertEqual( doc["example.p.1.s.1.w.19"]->text_content()->get_reference(off),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
#else
  assertEqual( doc["example.p.1.s.1.w.19"]->text_content()->get_reference(),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
#endif
  assertEqual( doc.get_warn_count(), 1 );
}

void text_test12(){
  startTestSerie( "Validation - Text Validation on String" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een <t-style class=\"emphasis\">volwaardig</t-style> literair oeuvre voorbehouden aan schrijvers<br/>\tals <t-str xlink:href=\"https://nl.wikipedia.org/wiki/Louis_Couperus\" xlink:type=\"simple\">Couperus</t-str>, 	Haasse, of"
"\tGrunberg?</t>"
"        <str xml:id=\"example.string\">"
"            <t offset=\"7\">creëren</t>"
"        </str>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
#if FOLIA_INT_VERSION >= 215
  int off = 0;
  assertEqual( doc["example.string"]->text_content()->get_reference(off),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
#else
  assertEqual( doc["example.string"]->text_content()->get_reference(),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
#endif

}

void text_test13a(){
  startTestSerie( "Validation - Text Validation on Correction (single text layer)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"          <t offset=\"50\">voorbehouden</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
 "</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
}

void text_test13b(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"original\">Is het creeren van een volwaardig litterair oeuvre voorbehouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"original\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"original\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"original\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"original\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"original\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"original\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"original\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"original\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"          <t offset=\"50\">voorbehouden</t>"
"          <t class=\"original\" offset=\"51\">voorbehouden</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"original\" offset=\"64\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"original\" offset=\"68\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"original\" offset=\"78\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
  "</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
}

void text_test13c(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers, structural changes)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"original\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"original\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"original\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"original\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"original\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"original\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"original\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"original\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"original\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"         <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"         </new>"
"         <original>"
"            <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"              <t class=\"original\" offset=\"51\">voor</t>"
"            </w>"
"            <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"              <t class=\"original\" offset=\"56\">behouden</t>"
"            </w>"
"         </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"original\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"original\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"original\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
}

void text_test13d(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers, structural changes, custom class)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"old\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"old\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"old\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"old\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"old\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"old\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"old\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"old\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"old\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"         <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"         </new>"
"         <original>"
"            <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"              <t class=\"old\" offset=\"51\">voor</t>"
"            </w>"
"            <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"              <t class=\"old\" offset=\"56\">behouden</t>"
"            </w>"
"         </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"old\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"old\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"old\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
}

void text_test13e(){
  startTestSerie( "Validation - Text Validation on complex nested Correction (Double text layers, structural changes, custom class)" );
  // NOTE: Current library implementation won't be able to validate nested layers and will just skip those!
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"old\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <t class=\"older\">Is het CREEREN van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"old\" offset=\"0\">Is</t>"
"          <t class=\"older\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"old\" offset=\"3\">het</t>"
"          <t class=\"older\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"             <correction>"
"               <new>"
"                 <t class=\"old\" offset=\"7\">creeren</t>"
"               </new>"
"               <original auth=\"no\">"
"                 <t class=\"older\" offset=\"7\">CREEREN</t>"
"               </original>"
"             </correction>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"old\" offset=\"15\">van</t>"
"          <t class=\"older\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"old\" offset=\"19\">een</t>"
"          <t class=\"older\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"old\" offset=\"23\">volwaardig</t>"
"          <t class=\"older\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"old\" offset=\"34\">litterair</t>"
"          <t class=\"older\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"old\" offset=\"44\">oeuvre</t>"
"          <t class=\"older\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"          <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"          </new>"
"          <original>"
"            <correction>"
"              <new>"
"                <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"                  <t class=\"old\" offset=\"51\">voor</t>"
"                </w>"
"                <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"                  <t class=\"old\" offset=\"56\">behouden</t>"
"                </w>"
"              </new>"
"              <original>"
"                <w xml:id=\"example.p.1.s.1.w.9c\" class=\"WORD\">"
"                  <t class=\"older\" offset=\"51\">voor</t>"
"                </w>"
"                <w xml:id=\"example.p.1.s.1.w.9d\" class=\"WORD\">"
"                  <t class=\"older\" offset=\"56\">behouden</t>"
"                </w>"
"              </original>"
"            </correction>"
"          </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"old\" offset=\"65\">aan</t>"
"          <t class=\"older\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"old\" offset=\"69\">schrijvers</t>"
"          <t class=\"older\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"old\" offset=\"79\">?</t>"
"          <t class=\"older\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
}

void text_test13f(){
  startTestSerie( "Validation - Text Validation with redundancy on construction" );
  Document doc( "xml:id""='example', version='1.5'" );
  KWargs args;
  args["xml:id"] = doc.id() + ".text.1";
  Text *text = new Text(args,&doc);
  doc.addText( text );

  args["xml:id"] = doc.id() + ".s.1";
  Sentence *s = new Sentence( args, &doc );
  s->settext( "De site staat online . " ); //Spaces here!
  text->append( s );
  args["xml:id"] = doc.id() + ".s.1.w.1";
  args["text"] =  "De";
  Word *w = new Word( args, &doc );
  s->append( w );
  args["xml:id"] = doc.id() + ".s.1.w.2";
  args["text"] =  "site";
  w = new Word( args, &doc );
  s->append( w );
  args["xml:id"] = doc.id() + ".s.1.w.3";
  args["text"] =  "staat";
  w = new Word( args, &doc );
  s->append( w );
  args["xml:id"] = doc.id() + ".s.1.w.4";
  args["text"] =  "online";
  w = new Word( args, &doc );
  s->append( w );
  args["xml:id"] = doc.id() + ".s.1.w.5";
  args["text"] =  "."; // No spaces here!
  w = new Word( args, &doc );
  s->append( w );

  doc.setmode("strip");
  assertEqual( doc.getmode(), "mode=strip,checktext,autodeclare," );
  assertEqual( doc.xmlstring(),
	       "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	       "<FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"\" version=\"\"><metadata type=\"native\"><annotations><text-annotation set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/text.foliaset.ttl\"/></annotations></metadata><text xml:id=\"example.text.1\"><s xml:id=\"example.s.1\"><t>De site staat online . </t><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><w xml:id=\"example.s.1.w.4\"><t>online</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s></text></FoLiA>\n" );
}

void text_test14(){
  startTestSerie( "Validation - Text Validation with sentence text delimiter inheritance" );
  Document doc;
  assertNoThrow( doc.read_from_file( "tests/textproblem.xml" ) );
}

void text_test15(){
  startTestSerie( "Validation - Whitespace in text content sanity check" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <metadata type=\"native\">"
"    <annotations>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"test.t\">"
"      <s xml:id=\"test.s\"><t>Dit\n"
"         is een rare test.\n"
"         </t>"
"      </s>"
"    </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
  assertEqual( doc["test.s"]->text(), "Dit is een rare test.");
}

void text_test16(){
  startTestSerie( "Validation - comment in text content sanity check" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <metadata type=\"native\">"
"    <annotations>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"test.t\">"
"      <t>This is the real text<comment annotator=\"pkampschreur\" annotatortype=\"manual\" datetime=\"2017-11-01T20:55:50\">Overbodig</comment>.</t>"
"    </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
  UnicodeString txt;
  assertNoThrow( txt = doc["test.t"]->text() );
  assertEqual( txt, "This is the real text.");
}

void text_test17(){
  startTestSerie( "Validation - mixed NFC and NFD encoding" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <text xml:id=\"t1\">"
"   <s xml:id=\"s1\">"
"    <t>ἀντιϰειμένου</t>"
"      <w xml:id=\"w1\">"
"        <t>ἀντιϰειμένου</t>"
"      </w>"
"    </s>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.read_from_string( xml ) );
  assertEqual( doc.sentences(0)->text(), "ἀντιϰειμένου");
  assertEqual( doc.words(0)->text(), "ἀντιϰειμένου");
}

void create_doc( bool with_space ){
  Document doc( "xml:id='style'" );
  assertNoThrow( doc.declare( AnnotationType::PARAGRAPH, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::STYLE, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::HYPHENATION, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::STRING, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::METRIC, "myset" ) );

  KWargs args;
  args["xml:id"] = "text";
  FoliaElement *root = doc.addText( args );
  args.clear();
  args["generate_id"] = root->id();
  Paragraph *p = new Paragraph( args, &doc );
  root->append( p );
  args.clear();
  TextContent *t = new TextContent( args, &doc );
  p->append( t );
  args["generate_id"] = p->id();
  TextMarkupString *str = new TextMarkupString( args, &doc );
  t->append( str );
  args.clear();
  args["text"] = "deel";
  TextMarkupStyle *style = new TextMarkupStyle( args, &doc );
  str->append( style );
  KWargs margs;
  margs["class"] = "some";
  margs["subset"] = "things";
  Feature *f = new Feature( margs, &doc );
  style->append( f );
  Hyphbreak *hbr = new Hyphbreak();
  style->append( hbr );
  args.clear();
  args["generate_id"] = p->id();
  str = new TextMarkupString( args, &doc );
  t->append( str );
  args.clear();
  args["text"] = "woord";
  style = new TextMarkupStyle( args, &doc );
  str->append( style );
  margs["class"] = "other";
  margs["subset"] = "things";
  f = new Feature( margs, &doc );
  style->append( f );
  if ( with_space ){
    XmlText *txt = new folia::XmlText();
    txt->setvalue( " " );
    t->append( txt );
    args["text"] = "extra";
  }
  else {
    args["text"] = " extra";
  }
  str = new TextMarkupString( args, &doc );
  t->append( str );
  if ( with_space ){
    assertNoThrow( doc.save( "/tmp/styles-with-space.xml" ) );
  }
  else {
    assertNoThrow( doc.save( "/tmp/styles-no-space.xml" ) );
  }
}

void create_newstyle_doc( bool with_space ){
  Document doc( "xml:id='style'" );
  assertNoThrow( doc.declare( AnnotationType::PARAGRAPH, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::STYLE, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::HYPHENATION, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::STRING, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::METRIC, "myset" ) );

  KWargs args;
  args["xml:id"] = "text";
  FoliaElement *root = doc.addText( args );
  args.clear();
  args["generate_id"] = "auto()";
  Paragraph *p = root->add_child<Paragraph>( args );
  args.clear();
  TextContent *t = p->add_child<TextContent>( args );
  args["xml:id"] = "auto()";
  TextMarkupString *str = t->add_child<TextMarkupString>( args );
  KWargs s_args;
  s_args["text"] = "deel";
  TextMarkupStyle *style = str->add_child<TextMarkupStyle>( s_args );
  KWargs m_args;
  m_args["class"] = "some";
  m_args["subset"] = "things";
  style->add_child<Feature>( m_args );
  style->add_child<Hyphbreak>();
  str = t->add_child<TextMarkupString>( args );
  s_args["text"] = "woord";
  style = str->add_child<TextMarkupStyle>( s_args );
  m_args["class"] = "other";
  style->add_child<Feature>( m_args );
  args.clear();
  if ( with_space ){
    t->add_child<XmlText>( " " );
    args["text"] = "extra";
  }
  else {
    args["text"] = " extra";
  }
  t->add_child<TextMarkupString>( args );
  if ( with_space ){
    assertNoThrow( doc.save( "/tmp/new-styles-with-space.xml" ) );
  }
  else {
    assertNoThrow( doc.save( "/tmp/new-styles-no-space.xml" ) );
  }
}

void text_test18(){
  startTestSerie( "Text creation - styles and stuff" );
  create_doc( false );
  create_doc( true );
  create_newstyle_doc( false );
  create_newstyle_doc( true );
  int stat = system( "./tests/foliadiff.sh /tmp/new-styles-no-space.xml /tmp/styles-no-space.xml" );
  assertMessage( "/tmp/new-styles-no-space.xml /tmp/styles-no-space.xml differ",
   		 (stat == 0) );

}

void text_test19a(){
  startTestSerie( " Adding TextContents in different sets" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"2.5\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <text-annotation annotator=\"sloot\" set=\"set_a\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"text\">\n"
"    <div xml:id=\"div\">\n"
"      <head xml:id=\"head\"/>\n"
"    </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.read_from_string(xml) );
  assertNoThrow( doc.declare( AnnotationType::TEXT, "set_a" ) );
#if FOLIA_INT_VERSION >= 214
  assertThrow( doc.declare( AnnotationType::TEXT, "set_b"), DocumentError );
#else
  assertThrow( doc.declare( AnnotationType::TEXT, "set_b"), XmlError );
#endif
  if ( hasThrown() ){
    cerr << "Error message was: " << lastError() << endl;
  }
}

void text_test19b(){
  startTestSerie( " Adding TextContents in different sets" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"2.5\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <text-annotation annotator=\"sloot\" set=\"set_a\"/>\n"
"      <text-annotation annotator=\"sloot\" set=\"set_b\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"text\">\n"
"    <div xml:id=\"div\">\n"
"      <head xml:id=\"head\"/>\n"
"    </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
#if FOLIA_INT_VERSION >= 214
  assertThrow( doc.read_from_string(xml), DocumentError );
#else
  assertThrow( doc.read_from_string(xml), XmlError );
#endif
  if ( hasThrown() ){
    cerr << "Error message was: " << lastError() << endl;
  }
  return;
  //  Next code should start working later on
  FoliaElement *head = doc["head"];
  KWargs args;
  args["value"] = "Woord";
  args["class"] = "class_a";
  args["set"] = "set_a";
  TextContent *tc = 0;
  assertNoThrow( tc = new TextContent( args, &doc ) );
  assertNoThrow( head->append( tc ) );
  args["value"] = "Woord2";
  args["class"] = "class_a";
  args["set"] = "set_b"; // different set. OK
  assertNoThrow( tc = new TextContent( args, &doc ) );
  assertNoThrow( head->append( tc ) );
  args["value"] = "Woord3"; // same set and class not OK
  assertNoThrow( tc = new TextContent( args, &doc ) );
  assertThrow( head->append( tc ), DuplicateAnnotationError );
  assertNoThrow( doc.save( "/tmp/test19.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/test19.xml tests/test19.xml" );
  assertMessage( "/tmp/test19.xml tests/test19.xml differ!",
   		 (stat == 0) );
}

void text_test20(){
  startTestSerie( "Validation - sentence ending with correction" );
  Document doc;
  assertNoThrow( doc.read_from_file( "tests/corrected.xml" ) );
}

void text_test21(){
  startTestSerie( "Validation - empty t-hbr" );
#if FOLIA_INT_VERSION >= 215
  {
    Document doc;
    assertNoThrow( doc.read_from_file("tests/bug52.xml") );
  }
  {
    Document doc;
    assertNoThrow( doc.read_from_file("tests/bug52-2.xml") );
  }
  {
    Document doc;
    assertThrow( doc.read_from_file("tests/bug52-3.xml"),
		 UnresolvableTextContent );
  }
#else
  Document doc;
  assertThrow( doc.read_from_file("tests/bug52.xml"),
	       UnresolvableTextContent );
#endif
}
