/*
 *      EGADS: Electronic Geometry Aircraft Design System
 *
 *             EGADS Lite Read & Tester
 *
 *      Copyright 2011-2021, Massachusetts Institute of Technology
 *      Licensed under The GNU Lesser General Public License, version 2.1
 *      See http://www.opensource.org/licenses/lgpl-2.1.php
 *
 */

#include "egads.h"
#include <assert.h>

#ifdef WIN32
#define LONG long long
#else
#define LONG long
#endif

void ptEval(ego context, ego model) {
  //class_id 5 class_dim 2 oclass 23 isEdge 0 isFace 1 pt 0.750 0.750 0.500 clPt //1.000 1.000 0.000
  const int mymagic = model->magicnumber;
  printf("topo magicnum %d magic %d\n", mymagic, MAGIC);
  ego geom, *bodies;
  int oclass, mtype, nbodies, *senses;
  int stat = EG_getTopology(model, &geom, &oclass, &mtype, NULL, &nbodies, &bodies, &senses);
  assert(stat == EGADS_SUCCESS);
  printf("stat %d oclass %d mtype %d nbodies %d\n", stat, oclass, mtype, nbodies);
  for (int i = 0; i < nbodies; i++) {
    int n, j;
    ego *faces;
    stat = EG_getBodyTopos(bodies[i], NULL, FACE, &n, &faces);
    assert(stat == EGADS_SUCCESS);
    for (j = 0; j < n; j++) {
      int k = EG_indexBodyTopo(bodies[i], faces[j]);
      assert(k==j+1);
      ego egFace = faces[j];
      assert(egFace->oclass == FACE);
      if(k==5) {
        double ignored[3] = {0, 0, 0};
        double pt[3] = {0.75, 0.75, 0};
        double clPt[3] = {0.75, 0.75, 0};
        stat = EG_invEvaluate(egFace, pt, ignored, clPt);
        assert(stat == EGADS_SUCCESS);
        printf("clPt %.3f %.3f %.3f\n", clPt[0], clPt[1], clPt[2]);

        {
        double pt[3] = {0.0, 0.0, 0.0};
        double clPt[3] = {0.0, 0.0, 0};
        stat = EG_invEvaluate(egFace, pt, ignored, clPt);
        assert(stat == EGADS_SUCCESS);
        printf("clPt %.3f %.3f %.3f\n", clPt[0], clPt[1], clPt[2]);
        }

        {
        double pt[3] = {0.25, 0.25, 0.25};
        double clPt[3] = {0.25, 0.25, 0.25};
        stat = EG_invEvaluate(egFace, pt, ignored, clPt);
        assert(stat == EGADS_SUCCESS);
        printf("clPt %.3f %.3f %.3f\n", clPt[0], clPt[1], clPt[2]);
        }
        {
        //CoG as reported by ESP vGeom utility
        double pt[3] = {0.575587, 0.575587, 0.66667};
        double clPt[3] = {0.575587, 0.575587, 0.66667};
        stat = EG_invEvaluate(egFace, pt, ignored, clPt);
        assert(stat == EGADS_SUCCESS);
        printf("clPt %.3f %.3f %.3f\n", clPt[0], clPt[1], clPt[2]);
        }
      }
    }
    EG_free(faces);
  }
}


int main(int argc, char *argv[])
{
  ego    context, model;
  
  if (argc != 2) {
    printf(" Usage: liteTest liteFile\n\n");
    exit(EXIT_FAILURE);
  }
  /* initialize */
  printf(" EG_open          = %d\n", EG_open(&context));
  printf(" EG_loadModel     = %d  %s\n", EG_loadModel(context, 0, argv[1],
                                                      &model), argv[1]);

  ptEval(context, model);
  return 0;
 
  printf(" EG_close         = %d\n", EG_close(context));
  return 0;
}
