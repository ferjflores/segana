#Field SQL norevisions
By default  Drupal 7 creates revision tables for all fields. 
There are times when storing revisions is unecessary and a waste of HD space. 
For example, if your site uses a sass based services for loading nodes, users, or other entities. 

Field SQL norevision fixes this problem, by giving you the choice disable revisions for specific entites and bundles.
It also allows you to batch delete existing revisions.

This module can be used on new and existing sites.
In case of existing sites there are some steps you need to do, all explained below.o


##Installation
###New Sites
Download and enable the module as usual. Once enabled visit
admin/config/system/field_sql_norevisions/field_sql_norevisions_entity_settings
and select the entities that you want to disable revisioning.

###Existing Sites
####Install
Download and enable the module as usual. Once enabled visit
admin/config/system/field_sql_norevisions/field_sql_norevisions_entity_settings
and select the entities that you want to disable revisioning.

####Mass Delete Revision Data
In order to mass-delete revisions on an existing site visit
admin/config/system/field_sql_norevisions/field_sql_norevisions_batch_delete.
NOTE: There is no turning back at this point, once you remove the revisions, they are gone forever.


##Useful Queries for Before/After
The following query will give you the total size of your database(s):

  SELECT table_schema "Database Name",
      sum( data_length + index_length ) / 1024 / 1024 "Data Base Size in MB",
      sum( data_free )/ 1024 / 1024 "Free Space in MB"
  FROM information_schema.TABLES
  GROUP BY table_schema ; 

Where as the following one will give you the size of the tables inside the database.
Change DATABASE_NAME with your database.

  SELECT table_name AS "Table",
  ROUND(((data_length + index_length) / 1024 / 1024), 2) AS "Size (MB)"
  FROM information_schema.TABLES
  WHERE table_schema = "DATABASE_NAME"
  ORDER BY (data_length + index_length) DESC;

Finally, if you want to get the sum of how much size the revision tables are using in your DB try this query:

  SELECT SUM(ROUND(((data_length + index_length) / 1024 / 1024), 2)) AS "Size (MB)" FROM information_schema.TABLES WHERE table_schema = "DATABASE_NAME" AND TABLE_NAME LIKE 'field_revision%' ORDER BY (data_length + index_length) DESC;

##Usefull Links
http://posulliv.github.io/2013/01/08/norevisions-field/
http://www.ambidev.com/make-your-drupal-7-faster-by-removing-all-revisions/
