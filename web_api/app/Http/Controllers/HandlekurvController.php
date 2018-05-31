<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Handlekurv;
use App\Vare;

class HandlekurvController extends Controller
{
	public function ListAll() {
		$varer = Handlekurv::all();
		$pris = 0;
		foreach ($varer as $vare) {
			$pris += $vare->antall * $vare->varedata['pris'];
		}
		return view ('handlekurv', compact('varer','pris'));
	}

	public function LeggTil($kunde, $upc) {
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

	public function LeggTilPost(Request $request) {
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

	public function BestillingOK($kunde) {
		Handlekurv::where('kunde','=',$kunde)->delete();
		return view ('bestilt');
	}
}
