<?php
namespace PGGI;
chdir(__DIR__);



// utils
function AddMenuItem($menu, $name = NULL, $callback = NULL, $param = NULL){
	$item = new GMenuItem($name);
	if($callback)
		$item->on(SIGNAL_GMENUITEM_ACTIVATE, $callback, $param);
	$menu->add($item);
}

class G2048{
	private $table, $grid, $labels, $score;
	
	public function __construct($app){
		$grid = new GGrid();
		$this->grid = $grid;
		$bar = new GMenuBar();

		$fileMenu = new GMenu();
		AddMenuItem($fileMenu, "quit", function($item, $data){
			$data->quit();
		}, $app);

		$item = new GMenuItem("File");
		$item->submenu = $fileMenu;
		$bar->add($item);

		$helpMenu = new GMenu();
		AddMenuItem($helpMenu, "about", function($item, $data){
			$data->about_dialog = new GAboutDialog();
			$data->about_dialog->programName = "php reflection reader";
			$data->about_dialog->copyright = "Valentin Collet - 2017";
			$data->about_dialog->licenseType = GAboutDialog::LICENSE_BSD;
			$data->about_dialog->version = "1.0";
			$data->about_dialog->comments = "the famous 2048 game made with pggi, for all question please send me an email at <valentin@famillecollet.com> .";
			$data->about_dialog->translatorCredits = "Valentin Collet";
			$data->about_dialog->logoIconName = "logo icon name";
			$data->about_dialog->website = "https://github.com/Letargie/pggi";
			$data->about_dialog->websiteLabel = "sources sur github";
			$data->about_dialog->run();
		}, $this);

		$item = new GMenuItem("Help");
		$item->submenu = $helpMenu;
		$bar->add($item);

		
		$box = new GBox(ORIENTATION_VERTICAL);
		$box->packStart($bar);
		$box->packStart($grid, true, true);
		$box->marginStart = $box->marginTop = $box->marginBottom = $box->marginEnd = 10;
		$w = new GWindow($app, "G2048");
		$w->setDefaultSize(1000, 700);
		$w->add($box);
		
		$w->on(SIGNAL_GWIDGET_KEY_PRESS_EVENT, function($window, $data, $event){
			switch($event->keyval){
				case GEventKey::KEY_Up:
				case GEventKey::KEY_W:
				case GEventKey::KEY_w:
					$this->moveUp();
					break;
				case GEventKey::KEY_Down:
				case GEventKey::KEY_S:
				case GEventKey::KEY_s:
					$this->moveDown();
					break;
				case GEventKey::KEY_Left:
				case GEventKey::KEY_A:
				case GEventKey::KEY_a:
					$this->moveLeft();
					break;
				case GEventKey::KEY_Right:
				case GEventKey::KEY_D:
				case GEventKey::KEY_d:
					$this->moveRight();
					break;
			}
		}, $this);

		$this->initTable();

		for($y = 0; $y < 4; ++$y)
			for($x = 0; $x < 4; ++$x){
				$digit = " ";
				if($this->table[$y][$x])
					$digit.= $this->table[$y][$x];
				$this->labels[$y][$x] = new GLabel($digit);
				$grid->attach($this->labels[$y][$x], $x, $y, 1, 1);
				$this->labels[$y][$x]->widthChars = 15;
				$this->labels[$y][$x]->show();
			}
		$screen = $w->getScreen();
		$provider = new GCssProvider();
		$provider->loadFromPath("style.css");
		$screen->addProvider($provider, GStyleContext::PRIORITY_USER);

		$grid->showAll();
		$w->showAll();
	}

	private function initTable(){
		$this->table = array();
		$this->labels = array();
		for($i = 0; $i < 4; ++$i){
			$this->table[] = array(0, 0, 0, 0);
			$this->labels[] = array();
		}
		$score = 0;
		$this->insertADigit();
		$this->insertADigit();
	}

	private function insertADigit(){
		$linear_index = $this->randomLocation();
		$this->table[$linear_index / 4][$linear_index % 4] = (rand(0, 9) == 0 ? 4 : 2);
	}

	private function randomLocation(){
		$index = 0;
		$count = 0;
		while($index <= 15){
			if($this->table[$index / 4][$index % 4] == 0)
				++$count;
			++$index;
		}
		$empty_location = array();
		$index = 0;
		$empty_index = 0;

		while($index <= 15){
			if($this->table[$index / 4][$index % 4] == 0){
				$empty_location[$empty_index] = $index;
				++$empty_index;
			}
			++$index;
		}
		return $empty_location[rand(0, $count-1)];
	}

	private function copyTable(){
		$tor = array();
		foreach($this->table as $val){
			$tor[] = $val;
		}
		return $tor;
	}

	private function moveUp(){
		$beforeUp = $this->copyTable();
		$this->upRemoveBlank();
		for($x = 0; $x < 4; ++$x){
			// Fusions
			if($this->table[0][$x] == $this->table[1][$x]){
				$this->table[0][$x] *= 2;
				$this->table[1][$x] = 0;
				$this->score += $this->table[0][$x];
				if($this->table[2][$x] == $this->table[3][$x]){
					$this->table[2][$x] *= 2;
					$this->table[3][$x] = 0;
					$this->score += $this->table[2][$x];
				}
			}else if($this->table[1][$x] == $this->table[2][$x]){
				$this->table[1][$x] *= 2;
				$this->table[2][$x] = 0;
				$this->score += $this->table[1][$x];
			}else if($this->table[2][$x] == $this->table[3][$x]){
				$this->table[2][$x] *= 2;
				$this->table[3][$x] = 0;
				$this->score += $this->table[2][$x];
			}
		}
		$this->upRemoveBlank();
		if($this->changed($beforeUp))
			$this->doIfChanged();
	}

	private function moveDown(){
		$beforeDown = $this->copyTable();
		$this->downRemoveBlank();
		for($x = 0; $x < 4; ++$x){
			if($this->table[3][$x] == $this->table[2][$x]){
				$this->table[3][$x] *= 2;
				$this->table[2][$x] = 0;
				$this->score += $this->table[3][$x];
				if($this->table[1][$x] == $this->table[0][$x]){
					$this->table[1][$x] *= 2;
					$this->table[0][$x] = 0;
					$this->score += $this->table[1][$x];
				}
			}else if($this->table[2][$x] == $this->table[1][$x]){
				$this->table[2][$x] *= 2;
				$this->table[1][$x] = 0;
				$this->score += $this->table[2][$x];
			}else if($this->table[1][$x] == $this->table[0][$x]){
				$this->table[1][$x] *= 2;
				$this->table[0][$x] = 0;
				$this->score += $this->table[1][$x];
			}
		}
		$this->downRemoveBlank();
		if($this->changed($beforeDown))
			$this->doIfChanged();
	}

	private function moveRight(){
		$beforeRight = $this->copyTable();
		$this->rightRemoveBlank();
		for($y = 0; $y < 4; ++$y){
			if($this->table[$y][3] == $this->table[$y][2]){
				$this->table[$y][3] *= 2;
				$this->table[$y][2] = 0;
				$this->score += $this->table[$y][3];
				if($this->table[$y][1] == $this->table[$y][0]){
					$this->table[$y][1] *= 2;
					$this->table[$y][0] = 0;
					$this->score += $this->table[$y][1];
				}
			}else if($this->table[$y][2] == $this->table[$y][1]){
				$this->table[$y][2] *= 2;
				$this->table[$y][1] = 0;
				$this->score += $this->table[$y][2];
			}else if($this->table[$y][1] == $this->table[$y][0]){
				$this->table[$y][1] *= 2;
				$this->table[$y][0] = 0;
				$this->score += $this->table[$y][1];
			}
		}
		$this->rightRemoveBlank();
		if($this->changed($beforeRight))
			$this->doIfChanged();
	}

	private function moveLeft(){
		$beforeLeft = $this->copyTable();
		$this->leftRemoveBlank();
		for($y = 0; $y < 4; ++$y){
			if($this->table[$y][0] == $this->table[$y][1]){
				$this->table[$y][0] *= 2;
				$this->table[$y][1] = 0;
				$this->score += $this->table[$y][0];
				if($this->table[$y][2] == $this->table[$y][3]){
					$this->table[$y][2] *= 2;
					$this->table[$y][3] = 0;
					$this->score += $this->table[$y][2];
				}
			}else if($this->table[$y][1] == $this->table[$y][2]){
				$this->table[$y][1] *= 2;
				$this->table[$y][2] = 0;
				$this->score += $this->table[$y][1];
			}else if($this->table[$y][2] == $this->table[$y][3]){
				$this->table[$y][2] *= 2;
				$this->table[$y][3] = 0;
				$this->score += $this->table[$y][2];
			}
		}
		$this->leftRemoveBlank();
		if($this->changed($beforeLeft))
			$this->doIfChanged();
	}

	private function upRemoveBlank(){
		for($x = 0; $x < 4; ++$x)
			for($i = 0; $i < 3; ++$i)
				for($y = 1; $y < 4; ++$y)
					if($this->table[$y-1][$x] == 0 && $this->table[$y][$x] != 0){
						$temp = $this->table[$y-1][$x];
						$this->table[$y-1][$x] = $this->table[$y][$x];
						$this->table[$y][$x] = $temp;
					}
	}

	private function downRemoveBlank(){
		for($x = 0; $x < 4; ++$x)
			for($i = 0; $i < 3; ++$i)
				for($y = 2; $y >= 0; --$y)
					if($this->table[$y+1][$x] == 0 && $this->table[$y][$x] != 0){
						$temp = $this->table[$y+1][$x];
						$this->table[$y+1][$x] = $this->table[$y][$x];
						$this->table[$y][$x] = $temp;
					}
	}

	private function leftRemoveBlank(){
		for($y = 0; $y < 4; ++$y)
			for($i = 0; $i < 3; ++$i)
				for($x = 1; $x < 4; ++$x)
					if($this->table[$y][$x-1] == 0 && $this->table[$y][$x] != 0){
						$temp = $this->table[$y][$x-1];
						$this->table[$y][$x-1] = $this->table[$y][$x];
						$this->table[$y][$x] = $temp;
					}
	}

	private function rightRemoveBlank(){
		for($y = 0; $y < 4; ++$y)
			for($i = 0; $i < 3; ++$i)
				for($x = 2; $x >= 0; --$x)
					if($this->table[$y][$x+1] == 0 && $this->table[$y][$x] != 0){
						$temp = $this->table[$y][$x+1];
						$this->table[$y][$x+1] = $this->table[$y][$x];
						$this->table[$y][$x] = $temp;
					}
	}

	private function doIfChanged(){
		$this->insertADigit();
		$this->show();
		echo($this->score."\n");
		if(!$this->canMove()){
			/*GtkWidget *parent = ((struct wnd_grid *)ptr_wnd_widget)->window;
			GtkWidget *msgdlg =
		  	gtk_message_dialog_new(GTK_WINDOW(parent),
				                 GTK_DIALOG_MODAL,
				                 GTK_MESSAGE_INFO,
				                 GTK_BUTTONS_YES_NO,
				                 "Can not move anymore, \n"
				                 " Press yes to play again, no to quit");
			gint result = gtk_dialog_run(GTK_DIALOG(msgdlg));
			switch (result)
			{
			case GTK_RESPONSE_YES:
			init_table();
			gtk_widget_destroy(msgdlg);
			show(GTK_GRID (grid));
			break;
			case GTK_RESPONSE_NO:
			  gtk_widget_destroy(msgdlg);
			gtk_widget_destroy(parent);
			break;
			}
			}
			}*/
			printf("PERDU!\n");
		}

	}

	private function show(){
		for($y = 0; $y < 4; ++$y)
			for($x = 0; $x < 4; ++$x){
				$text = " ";
				if($this->table[$y][$x])
					$text.= $this->table[$y][$x];
				// not working yet
				//$label = $this->grid->getChildAt($x, $y);
				$this->labels[$y][$x]->text = $text;
			}
	}

	private function canMove(){
		for($y = 0; $y < 4; ++$y)
			for($x = 0; $x < 4; ++$x)
				if(!$this->table[$y][$x])
					return true;
		for($y = 0; $y < 4; ++$y)
			for($x = 0; $x < 4; ++$x){
				// left
				if ($x > 0 && $this->table[$y][$x-1] == $this->table[$y][$x])
					return true;
				// right
				if ($x < 3 && $this->table[$y][$x+1] == $this->table[$y][$x])
					return true;
				// up
				if ($y > 0 && $this->table[$y-1][$x] == $this->table[$y][$x])
					return true;
				// down
				if ($y < 3 && $this->table[$y+1][$x] == $this->table[$y][$x])
					return true;
			}
		return false;
	}

	
	private function changed($a){
		for($y = 0; $y < 4; ++$y)
			for($x = 0; $x < 4; ++$x)
				if($a[$y][$x] != $this->table[$y][$x])
					return true;
		return false;
	}
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,function($app, $data){
	$app->reader = new G2048($app);
});
$a->run();


