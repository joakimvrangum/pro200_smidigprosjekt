<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Handlekurv;
use App\Vare;

class HandlekurvController extends Controller
{
	/**
		Metode for å liste opp alle varer i handlekurven, og vise disse..
	**/
	public function ListAll() {
		$varer = Handlekurv::all();
		$pris = 0;
		foreach ($varer as $vare) {
			$pris += $vare->antall * $vare->varedata['pris'];
		}
		return view ('handlekurv', compact('varer','pris'));
	}
	

	/**
		Metode for å legge til varer i handelkurven fra scanneren via GET-request.
	**/
	public function LeggTilGetRequest($kunde, $upc) {
		if (Vare::find($upc)) {
			$handlekurv = Handlekurv::where('kunde','=',$kunde)->where('upc','=',$upc)->first();
			if ($handlekurv === NULL) {
				$handlekurv = New Handlekurv;
				$handlekurv->kunde = $kunde;
				$handlekurv->upc = $upc;
				$handlekurv->antall = 1;
				$handlekurv->save();
			} else {
				$handlekurv->antall++;
				$handlekurv->save();
			}
			return "OK";
		} else {
			return "FAIL";
		}
	}


	/**
		Metode for å legge til varer i handelkurven fra scanneren via POST-request.
	**/
	public function LeggTilPostRequest(Request $request) {
		$request->validate(
			[
				'upc'	=> 'required|max:14',
				'kunde'	=> 'required',
			],
			[
				'upc.required'		=>	'Strekkode mangler.',
				'upc.max'			=>	'Strekkoden er for lang.',
				'kunde.required'	=>	'Mangler kunde-ID.',
				'kunde.int' 		=>	'Kunde-ID har feil datatype.',
			]);

			if (Vare::find($request->upc)) {
				$handlekurv = Handlekurv::where('kunde','=',$request->kunde)->where('upc','=',$request->upc)->first();
				if ($handlekurv === NULL) {
					$handlekurv = New Handlekurv;
					$handlekurv->kunde = $request->kunde;
					$handlekurv->upc = $request->upc;
					$handlekurv->antall = 1;
					$handlekurv->save();
				} else {
					$handlekurv->antall++;
					$handlekurv->save();
				}
				return "OK";
			} else {
				return "FAILED_ADD";
			}
	}


	/**
		Metode for å sette bestilling til bestilt/OK.
	**/
	public function BestillingOK($kunde) {
		Handlekurv::where('kunde','=',$kunde)->delete();
		return view ('bestilt');
	}
}
