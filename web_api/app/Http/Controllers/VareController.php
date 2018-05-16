<?php

namespace App\Http\Controllers;
use Illuminate\Support\Facades\Storage;
use Illuminate\Http\Request;
use App\Vare;
use Validator;
use Image;

class VareController extends Controller
{
	protected $table = 'varer';

    public function new(Request $request) {
    	//dd($request);
		$validator = Validator::make($request->input(),
			[
				'upc'	=> 'required|max:14',
				'navn'	=> 'required',
				'pris'	=> 'required',
			],
			[
				'upc.required'		=>	'Strekkode mangler.',
				'upc.max'			=>	'Strekkoden er for lang.',
				'navn.required'		=>	'Varen mangler navn.',
				'pris.required' 	=>	'Varen må ha en pris!',
			]);
		if($validator->passes()) {
			Vare::create([
				'upc'				=>	$request->upc,
				'vare_navn'			=>	$request->navn,
				'vare_beskrivelse'	=>	$request->beskrivelse,
				'pris'				=>	$request->pris]);
			if ($request->hasFile('bilde')) {
				$this->LastOppBilde($request);
			}
			return redirect()->back()->with('message', 'Varen ble registrert!');
		} else {
			return redirect()->back()->withInput()->withErrors($validator);
		}
    }

	public function ListAll() {
		$varer = Vare::paginate(20);
		return view ('listall', compact('varer'));
	}

	public function LastOppBilde(Request $request)
	{
		$fil = $request->file('bilde');
		$path = Storage::disk('public')->put('bilde', $fil);
		Image::make(storage_path().'/app/public/'.$path)->fit(100)->save(storage_path().'/app/public/thumb/'.$path);
		$vare = Vare::find($request->upc);
		$vare->bilde = $path;
		$vare->save();
	}
}