void bulidMainList() {
  step act;
  step *last = NULL, *first = NULL, *help = NULL;

  while (fread(&act, sizeof(step), 1, gamefile) == 1) {
      help = (step*)malloc(sizeof(step));
      (*help )              = act;
      help->next            = NULL;

      if (last) last->next = help;
      help->before         = last;
      last                 = help;
      if (first == NULL) first = help;

  }
  mainlistH = first;
  mainlistE = help;

}


void saveList(step *first, char *fileName)
{
  step *help;
  FILE *fp;
  
  help = first;
  fp = fopen(fileName,"w+b");

  while (help != NULL){
      fwrite(help, sizeof(step), 1, fp);
      help = help->next;

    }
  fclose(fp);

}

