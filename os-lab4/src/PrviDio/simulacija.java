package PrviDio;

import java.util.Random;
import java.util.concurrent.TimeUnit;

public class simulacija {

	private static class forma{
		private Integer name = 0;
		private Integer takt = 0;
		
		private forma() {
			this.name = 0;
			this.takt = 0;
		}
		
		private void povecajTakt(){
			this.takt++;
		}
		private void resetirajTakt() {
			this.takt=0;
		}
		private void promjeniTakt(Integer takt) {
			this.takt = takt;
		}
		private void promjeniIme(Integer ime) {
			this.name = ime;
		}
		private Integer getName() {
			return this.name;
		}
		private Integer getTakt() {
			return this.takt;
		}
	}

	
	
	
	private static class popis{
		forma lista[] = new forma[100];
		
		private popis() {
			for(int i = 0; i < 100; i++) {
				lista[i] = new forma();
			}
		}
		
		private boolean dodaj(int vel) {
			int brojac=0;
			for(int i = 0; i < 100; i++) {
				if( lista[i].getName() == 0) {
					brojac++;
				}else {
					brojac=0;
				}
				if(brojac == vel) {
					while(brojac > 0) {
						lista[i-brojac+1].promjeniIme(vel);
						lista[i-brojac+1].povecajTakt();
						brojac--;
					}
					return true;
				}
			}
			return false;
		}
		
		private void preslozi() {
			System.out.println("PROSLO JE 7 TAKTOVA, PRESLAZEMO LISTU ");
			int promjena = 0;
			for(int i = 0; i < 100; i++) {
				if(lista[i].getName() == 0 ) {
					for(int j = i; j < 99; j++) {
						lista[j].promjeniIme(lista[j+1].getName());
						lista[j].promjeniTakt(lista[j+1].getTakt());
						if(lista[j].getName() != 0) {
							promjena = 1;
						}
					}
					if(promjena == 1) {
						i--;
						promjena=0;
					}
				}
			}
			this.print();
		}

		private void cisti() {
			int ime = 0;
			for(forma i : lista) {
				if(i.getTakt() == 5) {
					ime = i.getName();
					i.promjeniIme(0);
					i.resetirajTakt();
				}
			}
			if(ime != 0) {
				System.out.println("pobrisan podatak " + ime);
			}
		}

		public void povecajVrijeme() {
			for(forma i : lista) {
				if(i.getName() != 0) {
					i.povecajTakt();
				}
			}
			
		}

		public void print() {
			for(forma i : lista) {
				if(i.getName() == 0) {
					System.out.print("|   ");
				}else {
					System.out.print("| "+ i.getName()+" ");
				}
			}
			System.out.println();	
		}
		
	}
	
	
	
	
	public static void main(String[] args) throws InterruptedException{
		
		popis pop = new popis();
		Random rand = new Random();
		int vrijeme = 0;
		
		int velicina = 102;
		
		while(true) {
			pop.cisti();
			pop.print();
			TimeUnit.SECONDS.sleep((long) 0.5);
			pop.povecajVrijeme();
			TimeUnit.SECONDS.sleep(1);
			if(pop.dodaj(velicina)) {
				System.out.println("upisan novi podatak velicine " + velicina);
			}else {
				System.out.println("zahtjev velicine "+ velicina + " nije uspio.");
			}
			TimeUnit.SECONDS.sleep(1);
			pop.print();
			velicina = rand.nextInt(20)+1;
			vrijeme++;
			if(vrijeme == 7) {
				pop.preslozi();
				vrijeme = 0;
			}
		}
	}
}
