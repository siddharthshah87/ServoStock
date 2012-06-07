package com.neuronrobotics.replicator.gui.navigator;

import java.io.File;

public class WorkspaceLeafNodeObject {
	
	private File theSTLFile, theGCodeFile;
	
	public WorkspaceLeafNodeObject(File stl, File gcode){
		theSTLFile = stl;
		theGCodeFile = gcode;
	}

	public File getTheSTLFile() {
		return theSTLFile;
	}

	public void setTheSTLFile(File theSTLFile) {
		this.theSTLFile = theSTLFile;
	}

	public File getTheGCodeFile() {
		return theGCodeFile;
	}

	public void setTheGCodeFile(File theGCodeFile) {
		this.theGCodeFile = theGCodeFile;
	}
	
	public String toString(){
		return theSTLFile.getName();
	}

}
