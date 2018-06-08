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
		Metode for å legge til varer i handelkurven fra scanneren via GET-request. Brukt til testing og debugging,
		ferdig produksjonskode bruker POST.
	**/
	public function LeggTilGetRequest($boks_id, $upc) {
		if (Vare::find($upc)) {
			$handlekurv = Handlekurv::where('boks_id','=',$boks_id)->where('upc','=',$upc)->first();
			if ($handlekurv === NULL) {
				$handlekurv = New Handlekurv;
				$handlekurv->boks_id = $boks_id;
				$handlekurv->upc = $upc;
				$handlekurv->antall = 1;
				$handlekurv->save();
			} else {
				$handlekurv->antall++;
				$handlekurv->save();
			}
			return "OK";
		}
		else if ($upc == 'ANGRE_VARE') {
			$this->SlettSisteVare($boks_id);
		}
		else if ($upc == 'LEVERING') {
			$this->BestillingOK($boks_id);
			return "LEVERING BESTILT";
		}
		else if ($upc == 'TOM_KURV') {
			$this->BestillingOK($boks_id);
			return "TØMT HANDLEKURV";
		}
		else {
			return "FAIL";
		}
	}


	/**
		Metode for å legge til varer i handelkurven fra scanneren via POST-request.
	**/
	public function LeggTilPostRequest(Request $request) {
		$request->validate(
			[
				'upc'		=> 'required|max:14',
				'boks_id'	=> 'required',
			],
			[
				'upc.required'		=>	'Strekkode mangler.',
				'upc.max'			=>	'Strekkoden er for lang.',
				'boks_id.required'	=>	'Mangler boks-ID.',
				'boks_id.int' 		=>	'Boks-ID har feil datatype.',
			]);

			if (Vare::find($request->upc)) {
				$handlekurv = Handlekurv::where('boks_id','=',$request->boks_id)->where('upc','=',$request->upc)->first();
				if ($handlekurv === NULL) {
					$handlekurv = New Handlekurv;
					$handlekurv->boks_id = $request->boks_id;
					$handlekurv->upc = $request->upc;
					$handlekurv->antall = 1;
					$handlekurv->save();
				} else {
					$handlekurv->antall++;
					$handlekurv->save();
				}
				return "OK";
			}
			else if ($request->upc == 'ANGRE_VARE') {
				$this->SlettSisteVare($request->boks_id);
				return "ANGRE_VARE";
			}
			else if ($request->upc == 'LEVERING') {
				$this->BestillingOK($request->boks_id);
				return "LEVERING BESTILT";
			}
			else if ($request->upc == 'TOM_KURV') {
				$this->BestillingOK($request->boks_id);
				return "TØMT HANDLEKURV";
			}
			else {
				return "FAIL";
			}
	}


	/**
		Metode for å sette bestilling til bestilt/OK.
	**/
	public function BestillingOK($kunde) {
		Handlekurv::where('boks_id','=',$kunde)->delete();
		return view ('bestilt');
	}

	/**
		Metode for å angre siste vare lagt i handlekurven.
	**/
	public function SlettSisteVare($boks_id) {
		$siste_vare = Handlekurv::where('boks_id','=',$boks_id)->orderBy('updated_at', 'desc')->first();
		if ($siste_vare != NULL) {
			if (($siste_vare->updated_at != $siste_vare->created_at) && ($siste_vare->antall > 1)) {
				$siste_vare->antall--;
				$siste_vare->save();
			} else {
				$siste_vare->delete();
			}
		}
	}
}
