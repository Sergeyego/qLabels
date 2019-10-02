
#include "tools.h"

#define POINTS_PER_POINT    1.0 /* internal units are points. */
#define POINTS_PER_INCH    72.0
#define POINTS_PER_MM       2.83464566929
#define POINTS_PER_CM       (10.0*POINTS_PER_MM)
#define POINTS_PER_PICA     (1.0/12.0)

typedef struct {
	char     *name;
	double      points_per_unit;
} UnitTableEntry;

static UnitTableEntry unit_table[] = {

	/* These names are identical to the absolute length units supported in
	   the CSS2 Specification (Section 4.3.2) */

	/* [GL_UNITS_POINT] */   {(char *)"pt",      POINTS_PER_POINT},
	/* [GL_UNITS_INCH]  */   {(char *)"in",      POINTS_PER_INCH},
	/* [GL_UNITS_MM]    */   {(char *)"mm",      POINTS_PER_MM},
	/* [GL_UNITS_CM]    */   {(char *)"cm",      POINTS_PER_CM},
	/* [GL_UNITS_PICA]  */   {(char *)"pc",      POINTS_PER_PICA},

};
double unit_to_point(QString c, qreal scale) {
    int i;
    for(i=0;i<5;i++) {
		if(c.endsWith(unit_table[i].name, Qt::CaseInsensitive)) {
		    c.remove(unit_table[i].name, Qt::CaseInsensitive);
	    	return c.toDouble()*unit_table[i].points_per_unit*scale;
		}
    }    
    return c.toDouble()*scale;
}

double point_to_unit(double points, QString c, qreal scale) {
    int i;
    for(i=0;i<5;i++) {
		if(c.endsWith(unit_table[i].name, Qt::CaseInsensitive)) {
	    	return points*(1/unit_table[i].points_per_unit)*scale;
		}
    }    
    return c.toDouble()*scale;
}

