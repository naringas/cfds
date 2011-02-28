<?php
/*
 * @file
 * Custom class for PDF printing header footer
 */

class FEpdf extends TCPDF {
  protected $header_html;
  public function headerHTML($html) {
    $this->header_html = $html;
  }
  public function Header() {
    $this->writeHTML($this->header_html);
    $this->SetTopMargin($this->getY() - 3);
    $this->SetFontSize('10');
    $this->MultiCell(0, 0, $this->getPage(), 0, '', FALSE, 0, 153.5, 72);
  }

  public function Footer() {
    $this->setY(284);
    $this->SetFontSize(11);
    $this->writeHTML('<strong>Este documento es una impresion de un Comprobante Fiscal Digital.</strong>', TRUE, FALSE, FALSE, FALSE, 'C');
  }
}
