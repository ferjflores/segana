<?php
/**
 * Cron Job Helper/Wrapper
 *
 * User: mbrown
 * Date: 2/23/14
 * Time: 7:24 PM
 *
 * @package  Cron_Wrapper
 * @author   Abhinav Singh <admin@abhinavsingh.com>
 * @author   Matthew Brown <mbrown@xxxxxxx.com>
 * @license  MIT http://opensource.org/licenses/MIT
 * @version  Release: $Id$
 * @link     http://abhinavsingh.com/blog/2009/12/how-to-use-locks-in-php-cron-jobs-to-avoid-cron-overlaps/
 */
class cronHelper
{

    private static $_pid;

    function __construct() {}

    function __clone() {}
    
    /**
     * Check If Cron Running
     * 
     * @return bool Response
     */
    private static function _isRunning()
    {
        $pids = explode(PHP_EOL, `ps -e | awk '{print $1}'`);
        if (in_array(self::$_pid, $pids)) {
            return true;
        }
        return false;
    }


    /**
     * Lock Currently Running Cron
     * 
     * @param string $name Name For Job
     * 
     * @return integer|bool 
     */
    public static function lock($name = '')
    {
        $logger = klogger_log();
        // Lock Name
        $lock_file = LOCK_DIR.$name.LOCK_SUFFIX;
        // Lock File Present?
        if (file_exists($lock_file)) {

            // Is running?
            self::$_pid = file_get_contents($lock_file);

            // Already Running - Return False
            if (self::_isRunning()) {
                $logger->info("==".self::$_pid."== {$name} already in progress...");
                return false;

            } else {
                // Not Running Continue - Output Message
                $logger->info("==".self::$_pid."== Previous {$name} job died abruptly...");
            }
        }

        // Get PID & Write To Lock File
        self::$_pid = getmypid();
        file_put_contents($lock_file, self::$_pid);

        // Write Success To Log
        $logger->info("==".self::$_pid."== Lock acquired, processing the {$name} job...");
        
        // Return This PID
        return self::$_pid;
    }


    /**
     * Release Lock On Cron
     * 
     * @param string $name Job Name
     * 
     * @return bool
     */
    public static function unlock($name = '')
    {
        $logger = klogger_log();
        // Lock File Name
        $lock_file = LOCK_DIR.$name.LOCK_SUFFIX;

        // If File Exists - Unlink
        if (file_exists($lock_file)) {
            unlink($lock_file);
        }

        // Write Success To Log & Return
        $logger->info("==".self::$_pid."== Releasing {$name} lock...");
        return true;
    }
}


/**
 * Usage Example
 */
/*require 'cron.helper.php';

if(($pid = cronHelper::lock()) !== FALSE) {

	/*
	 * Cron job code goes here
	*/

/*	cronHelper::unlock();
}*/

?>