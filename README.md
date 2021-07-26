# cathild-history-Parser
C-Program to Read all Excel files from Historique Folder with additional Columns (chamber,occurrence ) (Kammer,Vorgang ) 
to create complete overall Pivottables for all Chambers and Dates. Good Statistics.

Select only necessary columns to Parse, change Dates automatically to universal Timestamp.

need MCP,libreoffice(headless) commands on linux to rename courbes.xls to courbes_chamber_occurence and convert files to csv.

Only two Comamnds in History Folder:

and then:
soffice --headless --convert-to csv *.xls

Create courbes.txt:
find ./ -name "*.csv" -exec ./test.exe {} \;
