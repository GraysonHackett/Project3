if (!strcmp(args[0],"path")){
		char *new_path = malloc(sizeof(char)*1000);
		new_path = args[1];
		snprintf(cur_path, sizeof(char)*1000,"%s%s",cur_path,new_path);
		setenv("$PATH",cur_path,1);
	}
