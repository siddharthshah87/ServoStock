package com.neuronrobotics.replicator.test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

import com.neuronrobotics.replicator.driver.NRPrinter;
import com.neuronrobotics.replicator.driver.PrinterStatus;
import com.neuronrobotics.replicator.driver.SliceStatusData;
import com.neuronrobotics.replicator.gui.PrinterStatusListener;
import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.ui.ConnectionDialog;

public class PrinterTest implements PrinterStatusListener {
	private PrinterTest() {
		DyIO.disableFWCheck();
		DyIO dyio = new DyIO();
		if(!ConnectionDialog.getBowlerDevice(dyio)) {
			System.out.println("Fail");
			System.exit(1);
		}
		NRPrinter printer = new NRPrinter(dyio);
		//NRPrinter printer = new NRPrinter(null);
		printer.addPrinterStatusListener(this);
		File gcode = new File("test.gcode");
		try {
			//This is time consuming
//			printer.slice(PrinterTest.class.getResourceAsStream("bottle.stl"),new FileOutputStream(gcode));
			//...
			if(printer.isReady())
				printer.print(new FileInputStream(gcode));
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			new PrinterTest();
		}catch (Exception ex) {
			ex.printStackTrace();
			System.exit(1);
		}
	}
	@Override
	public void printStatus(PrinterStatus psl) {
		System.out.println("Printer status: "+psl);
	}
	@Override
	public void sliceStatus(SliceStatusData ssd) {
		System.out.println("Printer status: "+ssd);
	}

}
